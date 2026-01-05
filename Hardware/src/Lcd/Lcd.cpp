#include "Lcd/Lcd.h"

//SPIClass spi_tft(2);


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setupLCD(){
   // SPI.begin(SPI_TFT_SCK_PIN, SPI_TFT_MISO_PIN, SPI_TFT_MOSI_PIN);    // Initialize SPI bus with custom pins

    tft.initR(INITR_BLACKTAB); 
    tft.setRotation(1);
    showStartupScreen();
}

void clearScreen() {
    tft.fillScreen(BLACK);
}

void showStartupScreen() {
   // clearScreen();
    delay(300);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    printCenter("SLOTIFY");
    delay(500);
}

void showEntranceAuthorized(int parkingSpot) {
    //clearScreen();
    tft.setTextSize(2);
    tft.setTextColor(GREEN);
    
    // Display "Welcome" centered
    printCenterAtY("Welcome", 30);
    
    // Display parking space suggestion
    tft.setTextSize(1);
    String spaceText = "Space: " + String(parkingSpot);
    printCenterAtY(spaceText, 70);
}

void showEntranceUnauthorized() {
    //clearScreen();
    tft.setTextSize(1);
    tft.setTextColor(RED);
    printCenter("Not authorized");
}

void showExitAuthorized(String price) {
   // clearScreen();
    tft.setTextSize(2);
    tft.setTextColor(GREEN);
    
    // Display price
    String priceText = "$" + price;
    printCenterAtY(priceText, 40);
    
    // Display goodbye message
    tft.setTextSize(1);
    printCenterAtY("Thank you!", 80);
    printCenterAtY("Goodbye", 100);
}

void showExitUnauthorized() {
  //  clearScreen();
    tft.setTextSize(1);
    tft.setTextColor(RED);
    printCenter("Not authorized");
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
