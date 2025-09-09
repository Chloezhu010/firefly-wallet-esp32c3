#include "nvs_flash.h" // non-volatile storage flash
#include "nvs.h" // non-volatile storage api
#include "esp_system.h" // system functions
#include "esp_random.h" // random number generator
#include "../../source/libs/ethers/src/ethers.h" // ethereum lib

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
  for (int i = 0; i < 32; i++) {
    Serial.print(private_key[i], HEX);
  }

  // store pk in NVS
  size_t private_key_size = sizeof(private_key);
  if (nvs_set_blob(wallet_handle, "test_private_key", private_key, private_key_size) != ESP_OK) {
    Serial.println("❌ Failed to store private key");
  }
  else {
    Serial.println("✅ Private key stored successfully");
    nvs_commit(wallet_handle); // commit value to flash
  }

  // verify key retrieval
  uint8_t retrieved_private_key[32];
  size_t retrieved_private_key_size = sizeof(retrieved_private_key);
  if (nvs_get_blob(wallet_handle, "test_private_key", retrieved_private_key, &retrieved_private_key_size) != ESP_OK) {
    Serial.println("❌ Failed to retrieve private key");
  }
  else {
    Serial.println("✅ Private key retrieved successfully");
    for (int i = 0; i < 32; i++) { // print retrieved private key in hex
      Serial.print(retrieved_private_key[i], HEX);
    }
    Serial.println();
    // compare retrieved private key with original private key
    if (memcmp(private_key, retrieved_private_key, 32) == 0) {
      Serial.println("✅ Private key retrieved successfully");
    }
    else {
      Serial.println("❌ Private key retrieved failed");
    }
  }

  /* Address derivation test */

  /* Transaction signing test */

  /* Performance & memory test */

  /* Cleanup*/
  nvs_close(wallet_handle); // close NVS handle
  nvs_flash_deinit();

}

void loop() {
  // put your main code here, to run repeatedly:

}
