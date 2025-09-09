#include "nvs_flash.h" // non-volatile storage flash
#include "nvs.h" // non-volatile storage api
#include "esp_system.h" // system functions
#include "esp_random.h" // random number generator
#include "../../source/libs/ethers/src/ethers.h" // ethereum lib
#include "../../source/libs/ethers/src/ethers.c"
#include "../../source/libs/ethers/src/keccak256.c"
#include "../../source/libs/ethers/src/uECC.c"
#include <string.h> // for memcmp
#include <stdio.h> // for printf
#include <unistd.h>

/* debug function */
void check_nvs_partition_info() {
  Serial.println("=== Checking NVS partition info ===");
  // get nvs partition info
  const esp_partition_t *nvs_partition = esp_partition_find_first(
    ESP_PARTITION_TYPE_DATA, 
    ESP_PARTITION_SUBTYPE_DATA_NVS, 
    NULL);
  if (nvs_partition != NULL) {
    Serial.printf("NVS partition size: %d bytes (%.1f KB)\n",
                nvs_partition->size, nvs_partition->size / 1024.0);
    Serial.printf("NVS partition address: %p\n", nvs_partition->address);
  }
}

/* debug function */
void check_nvs_usage() {
  Serial.println("=== Checking NVS usage ===");

  nvs_stats_t stats;
  esp_err_t err = nvs_get_stats(NULL, &stats);
  int used = stats.used_entries;
  int free = stats.free_entries;
  int available = stats.available_entries;
  int total = stats.total_entries;
  if (err == ESP_OK) {
    printf(
      "Count: Used Entries = (%d), "
      "Free Entries = (%d), "
      "Available Entries = (%d), "
      "All Entries = (%d)\n",
      used, free, available, total
    );
  }
  else
    Serial.println("Failed to get NVS usage stats");
}

/* helper function： print into in hex */
void print_hex(uint8_t *data, int length) {
  // print the data in hex one by one
  for (int i = 0; i < length; i++) {
    Serial.print(data[i], HEX);
  }
  // print a new line
  Serial.println();
}

void setup() {
  /* 1. init setup */
  Serial.begin(115200);
  Serial.println("Starting...");

  /* 2. init NVS flash */
  if (esp_err_t err = nvs_flash_init() != ESP_OK) {
    Serial.println("NVS flash init failed");
    return;
  }

  /* 3. NVS storage setup */
  // open NVS handle
  nvs_handle_t wallet_handle;
  if (nvs_open("wallet", NVS_READWRITE, &wallet_handle) != ESP_OK) {
    Serial.println("NVS open failed");
  }
  else
    Serial.println("NVS open successful");

  // Test NVS write/read
  uint32_t test_value = 123456;
  if (nvs_set_u32(wallet_handle, "test_value", test_value) != ESP_OK) { // store value
    Serial.println("NVS write failed");
  }
  else
  {
    Serial.println("NVS write successful");
    nvs_commit(wallet_handle); // commit value to flash
  }

  uint32_t read_value = 0;
  if (nvs_get_u32(wallet_handle, "test_value", &read_value) != ESP_OK) { // read value
    Serial.println("NVS read failed");
  }
  else
    Serial.println("NVS read successful");
  
  if (read_value != test_value) // check if read value is the same as the setted value
    Serial.println("❌ NVS read/write test FAILED");
  else
    Serial.println("✅ NVS read/write test PASSED");

  /* 4. Key generation & storage*/
  // generate test private key
  uint8_t private_key[32];

  for (int i = 0; i < 32; i++) {
    private_key[i] = esp_random() & 0xFF; // generate private key array
  }
  // NEVER print the real private key
  Serial.println("Test private key generated: "); // print private key in hex
  print_hex(private_key, 32);

  // store pk in NVS
  size_t private_key_size = 32;
  if (nvs_set_blob(wallet_handle, "test", private_key, private_key_size) != ESP_OK) {
    Serial.println("❌ Failed to store private key");
  }
  else {
    Serial.println("✅ Private key stored successfully");
    nvs_commit(wallet_handle); // commit value to flash
  }

  // verify key retrieval
  uint8_t retrieved_private_key[32];
  size_t retrieved_private_key_size = 32;
  if (nvs_get_blob(wallet_handle, "test", retrieved_private_key, &retrieved_private_key_size) != ESP_OK) {
    Serial.println("❌ Failed to retrieve private key");
  }
  else {
    Serial.println("✅ Private key retrieved successfully");
    print_hex(retrieved_private_key, 32);
    // compare retrieved private key with original private key
    if (memcmp(private_key, retrieved_private_key, 32) == 0) {
      Serial.println("✅ Private key retrieved successfully");
    }
    else {
      Serial.println("❌ Private key retrieved failed");
    }
  }

  /* Address derivation test */
  // generate address from pk
  uint8_t address[20];
  if (ethers_privateKeyToAddress(retrieved_private_key, address)) {
    Serial.println("✅ Address generated successfully");
    Serial.print("Raw address: 0x");
    print_hex(address, 20);
  }
  else {
    Serial.println("❌ Address generation failed");
  }

  // generate checksum address
  char checksum_address[43]; // "0x" + 40 bytes address + "\0"
  if (ethers_privateKeyToChecksumAddress(retrieved_private_key, checksum_address)) {
    Serial.println("✅ Checksum address generated successfully");
    Serial.print("Checksum address: ");
    Serial.println(checksum_address);
  } else {
    Serial.println("❌ Checksum address generation failed");
  }

  /* Transaction signing test */

  /* Performance & memory test */

  /* Cleanup*/
  nvs_close(wallet_handle); // close NVS handle
  nvs_flash_deinit();

}

void loop() {
  // put your main code here, to run repeatedly:

}
