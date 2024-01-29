#include "Image.h"
#include "StickerSheet.h"

int main() {
  //
  // Reminder:
  //   Before exiting main, save your creation to disk as myImage.png
  //
  Image alma; alma.readFromFile("../data/alma.png");
  Image i;    i.readFromFile("../data/i.png");
  Image penguin; penguin.readFromFile("../data/penguin.png");

  StickerSheet sticker_sheet(alma, 4);

  sticker_sheet.addSticker(penguin, 100, 200);
  sticker_sheet.addSticker(i, 200, 300);
  sticker_sheet.addSticker(i, 10, 10);
  sticker_sheet.addSticker(penguin, 30, 400);

  Image final_sticker = sticker_sheet.render();
  final_sticker.writeToFile("myImage.png");


  return 0;
}
