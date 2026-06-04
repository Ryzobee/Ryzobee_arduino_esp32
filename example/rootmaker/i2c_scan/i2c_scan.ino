#include <Wire.h>
#include "Ryzobee.h"

typedef struct {
    const char *name;
    uint8_t sda;
    uint8_t scl;
} i2c_scan_port_t;

static const i2c_scan_port_t ports[] = {
    {"PORT1", PORT1_SDA, PORT1_SCL},
    {"PORT2", PORT2_SDA, PORT2_SCL},
    {"PORT3", PORT3_SDA, PORT3_SCL},
    {"PORT4", PORT4_SDA, PORT4_SCL},
};

static const uint8_t PORT_COUNT = sizeof(ports) / sizeof(ports[0]);

void scan_i2c_port(const i2c_scan_port_t *port)
{
    Wire.begin(port->sda, port->scl);
    
    Serial.printf("\n========================================\n");
    Serial.printf("Scanning %s (SDA=%d, SCL=%d)...\n", port->name, port->sda, port->scl);
    Serial.printf("========================================\n");
    
    uint8_t found_count = 0;
    
    for (uint8_t addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        uint8_t error = Wire.endTransmission();
        
        if (error == 0) {
            Serial.printf("[%s] Found device at address: 0x%02X (%d)\n", port->name, addr, addr);
            found_count++;
        }
    }
    
    if (found_count == 0) {
        Serial.printf("[%s] No I2C devices found.\n", port->name);
    } else {
        Serial.printf("[%s] Total devices found: %d\n", port->name, found_count);
    }
    
    Wire.end();
}

void setup(void)
{
    Serial.begin(115200);
    while (!Serial) {
        delay(10);
    }
    
    delay(1000);
    
    Serial.println("\n\n========================================");
    Serial.println("     I2C Scanner - 4 Ports");
    Serial.println("========================================");
    
    for (uint8_t i = 0; i < PORT_COUNT; i++) {
        scan_i2c_port(&ports[i]);
        delay(100);
    }
    
    Serial.println("\n========================================");
    Serial.println("     Scan Complete!");
    Serial.println("========================================\n");
}

void loop(void)
{
    Serial.println("\nPress reset button to scan again...");
    delay(5000);
}
