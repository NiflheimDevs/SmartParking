#include "Lcd/Lcd.h"
#include "Led/Led.h"

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setupLCD(){
    tft.initR(INITR_BLACKTAB); 
    tft.fillScreen(ST77XX_BLACK);
    delay(300);
    tft.setTextSize(2);
    tft.setRotation(1);
    tft.setTextColor(ST77XX_WHITE);
    printCenter("Slotify");
}

void printCenter(String text) {
    int16_t x, y;
    uint16_t w, h;
  
    // Measure the text box
    tft.getTextBounds(text, 0, 0, &x, &y, &w, &h);
  
    // Compute centered position
    int16_t cx = (tft.width()  - w) / 2;
    int16_t cy = (tft.height() - h) / 2;
  
    tft.setCursor(cx, cy);
    tft.print(text);
}

void printCenterAtY(String text, int16_t yPos) {
    int16_t x, y;
    uint16_t w, h;
  
    // Measure the text box
    tft.getTextBounds(text, 0, 0, &x, &y, &w, &h);
  
    // Compute centered X position
    int16_t cx = (tft.width() - w) / 2;
  
    tft.setCursor(cx, yPos);
    tft.print(text);
}

void showRFIDAuthorized(int parkingSlot) {
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextSize(2);
    tft.setTextColor(ST77XX_GREEN);
    
    // Show "Welcome" at top
    printCenterAtY("Welcome", 10);
    
    // Show parking slot suggestion
    if (parkingSlot > 0) {
        tft.setTextSize(1);
        tft.setTextColor(ST77XX_CYAN);
        String slotLabel = "Parking Slot:";
        printCenterAtY(slotLabel, 45);
        
        tft.setTextSize(2);
        tft.setTextColor(ST77XX_YELLOW);
        String slotText = "P" + String(parkingSlot);
        printCenterAtY(slotText, 65);
    } else {
        tft.setTextSize(1);
        tft.setTextColor(ST77XX_WHITE);
        printCenterAtY("Please proceed", 50);
    }
}

void showRFIDNotAuthorized() {
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextSize(2);
    tft.setTextColor(ST77XX_RED);
    printCenter("Not Authorized");
}

void showExitPrice(String price) {
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextSize(1);
    tft.setTextColor(ST77XX_WHITE);
    
    // Show "Exit" label
    printCenterAtY("Exit", 10);
    
    // Show price
    tft.setTextSize(2);
    tft.setTextColor(ST77XX_YELLOW);
    String priceText = "$" + price;
    printCenterAtY(priceText, 50);
    
    tft.setTextSize(1);
    tft.setTextColor(ST77XX_WHITE);
    printCenterAtY("Thank You!", 80);
}

void showParkingStatus() {
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextSize(1);
    
    // Header with decorative line
    tft.setTextColor(ST77XX_CYAN);
    printCenterAtY("Parking Status", 2);
    
    // Draw a line under header
    tft.drawLine(0, 12, tft.width(), 12, ST77XX_CYAN);
    
    // Count full and empty spaces
    int fullCount = 0;
    int emptyCount = 0;
    String fullSpaces = "";
    String emptySpaces = "";
    
    for (int i = 0; i < PARKING_SPACES; i++) {
        if (getParkingSpaceStatus(i)) {
            fullCount++;
            if (fullSpaces.length() > 0) fullSpaces += ", ";
            fullSpaces += "P" + String(i + 1);
        } else {
            emptyCount++;
            if (emptySpaces.length() > 0) emptySpaces += ", ";
            emptySpaces += "P" + String(i + 1);
        }
    }
    
    // Display full spaces section
    int yPos = 18;
    tft.setTextColor(ST77XX_RED);
    String fullText = "Full: " + String(fullCount) + "/" + String(PARKING_SPACES);
    tft.setCursor(5, yPos);
    tft.print(fullText);
    
    yPos += 12;
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(5, yPos);
    if (fullSpaces.length() > 0) {
        // Display spaces in a compact format
        if (fullSpaces.length() <= 18) {
            tft.print(fullSpaces);
        } else {
            // Split into two lines if too long
            int mid = fullSpaces.length() / 2;
            int splitPos = fullSpaces.lastIndexOf(',', mid);
            if (splitPos > 0) {
                tft.print(fullSpaces.substring(0, splitPos));
                tft.setCursor(5, yPos + 10);
                tft.print(fullSpaces.substring(splitPos + 2));
            } else {
                tft.print(fullSpaces.substring(0, 18));
            }
        }
    } else {
        tft.setTextColor(ST77XX_GREEN);
        tft.print("All Available!");
    }
    
    // Draw separator line
    yPos = (fullSpaces.length() > 18) ? yPos + 22 : yPos + 15;
    tft.drawLine(0, yPos, tft.width(), yPos, ST77XX_BLUE);
    yPos += 5;
    
    // Display empty spaces section
    tft.setTextColor(ST77XX_GREEN);
    String emptyText = "Empty: " + String(emptyCount) + "/" + String(PARKING_SPACES);
    tft.setCursor(5, yPos);
    tft.print(emptyText);
    
    yPos += 12;
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(5, yPos);
    if (emptySpaces.length() > 0) {
        // Display spaces in a compact format
        if (emptySpaces.length() <= 18) {
            tft.print(emptySpaces);
        } else {
            // Split into two lines if too long
            int mid = emptySpaces.length() / 2;
            int splitPos = emptySpaces.lastIndexOf(',', mid);
            if (splitPos > 0) {
                tft.print(emptySpaces.substring(0, splitPos));
                tft.setCursor(5, yPos + 10);
                tft.print(emptySpaces.substring(splitPos + 2));
            } else {
                tft.print(emptySpaces.substring(0, 18));
            }
        }
    } else {
        tft.setTextColor(ST77XX_RED);
        tft.print("All Full!");
    }
}


