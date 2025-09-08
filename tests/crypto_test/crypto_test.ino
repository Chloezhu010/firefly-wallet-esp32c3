#include "nvs_flash.h" // non-volatile storage flash
#include "nvs.h" // non-volatile storage api
#include "esp_system.h" // system functions
#include "esp_random.h" // random number generator
#include "../../source/libs/ethers/src/ethers.h" // ethereum lib

void setup() {
  /* init */
  Serial.begin(115200);
  Serial.println("Starting...");

  /* init NVS flash */
  if (esp_err_t err = nvs_flash_init() != ESP_OK) {
    Serial.println("NVS flash init failed");
    return;
  }

  /* NVS storage setup */
  // open NVS handle
  nvs_handle_t wallet_handle;
  // esp_err_t err;
  if (nvs_open("wallet", NVS_READWRITE, &wallet_handle) != ESP_OK) {
    Serial.println("NVS open failed");
    // return err;
  }
  else
    Serial.println("NVS open successful");

  // Test NVS write/read
  uint32_t test_value = 123456;
  if (nvs_set_u32(wallet_handle, "test_value", test_value) != ESP_OK) {
    Serial.println("NVS write failed");
    // return err;
  }
  else
    Serial.println("NVS write successful");

  uint32_t read_value = 0;
  if (nvs_get_u32(wallet_handle, "test_value", &read_value) != ESP_OK) {
    Serial.println("NVS read failed");
    // return err;
  }
  else
    Serial.println("NVS read successful");
  
  if (read_value != test_value)
    Serial.println("❌ NVS read/write test FAILED");
  else
    Serial.println("✅ NVS read/write test PASSED");

  
    

  /* Key generation & storage*/
  // generate test secret key

  // store in NVS

  // verify key retrieval

  /* Address derivation test */

  /* Transaction signing test */

  /* Performance & memory test */

  /* Cleanup*/
  nvs_close(wallet_handle);
  nvs_flash_deinit();

}

void loop() {
  // put your main code here, to run repeatedly:

}
