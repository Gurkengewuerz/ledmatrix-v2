#include <AnimationMatrix.h>

bool AnimationMatrix::update() {
    if (!Animation::update()) return false;

    Serial.println("clear");
    this->display->clear(false);
    bool hasEmpty = false;

    for (int16_t col = 0; col <= this->display->getCols(); col++) {
        Serial.printf("checking %d\n", col);
        MatrixLine matrixLine = this->lines[col];
        if (matrixLine.isValid()) {
            Serial.println("col is valid");
            // Draw the pixels of the current line
            this->drawLine(col, &matrixLine);
            // Check whether the line is out of the screen and if so, remove it
            if (matrixLine.finished()) {
                Serial.println("col is finished");
                this->lines[col] = MatrixLine();
            }
        } else
            hasEmpty = true;
    }

    // Select a random line, save it and draw it
    if (hasEmpty) {
        uint8_t lineCol = random(0, this->display->getCols());
        // If at the random position isn't a line, we add one
        if (!this->lines[lineCol].isValid()) {
            Serial.printf("creating new line at %d\n", lineCol);
            this->lines[lineCol] = MatrixLine();
            this->lines[lineCol].init(this->display);
            this->drawLine(lineCol, &this->lines[lineCol]);
        }
    }

    // And finally we draw everything
    this->display->update();

    return true;
}

void AnimationMatrix::drawLine(uint8_t col, MatrixLine* matrixLine) {
    uint32_t* color = matrixLine->tick();
    for (int16_t row = 0; row < this->display->getRows(); row++) {
        this->display->setPixel(col + 1, row + 1, color[row], false);
    }
}

void AnimationMatrix::reset() {
    Animation::reset();
    this->updateEvery = 20;
    this->lines = new MatrixLine[this->display->getCols()];
}

const char* AnimationMatrix::getName() {
    return "Matrix";
}

MatrixLine::MatrixLine() {
    this->valid = false;
}

void MatrixLine::init(WS281X* display) {
    this->display = display;
    this->length = random(5, 15);
    this->fadingOutTiles = random(2, 4);
    this->closeWhiteColor = random(180, 240);
    this->firstTileColor = display->getColor(this->closeWhiteColor, this->closeWhiteColor, this->closeWhiteColor);
    this->row = 0;
    this->valid = true;
}

uint32_t* MatrixLine::tick() {
    this->line = new uint32_t[this->display->getRows()];

    for (int16_t i = 0; i < this->display->getRows(); i++) {
        // Here we skip the empty tiles of the trail
        if (i < this->row - this->length) continue;
        // Then we look out for the fading tiles
        if (i < this->row - (this->length - this->fadingOutTiles)) {
            uint8_t fadingStartPos = this->row - this->length;
            float fadingPos = (float)i - (float)this->fadingOutTiles;
            uint8_t fadingGreen = (uint8_t)(roundf((fadingPos / ((float)this->fadingOutTiles - 1)) * 100) + 75);
            line[i] = display->getColor(0, fadingGreen, 0);
            continue;
        }
        // Next are the fully green painted tiles
        if (i < this->row) line[i] = display->getColor(0, 255, 0);
        // And at the end we handle the gray / white tile
        if (i == this->row) line[i] = this->firstTileColor;
    }

    this->row++;
    return line;
}

bool MatrixLine::finished() {
    return this->row - this->length - 1 >= this->display->getRows();
}

bool MatrixLine::isValid() {
    return this->valid;
}
