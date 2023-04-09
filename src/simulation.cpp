#include <AnimationGradient.h>
#include <AnimationMatrix.h>
#include <AnimationScanLine.h>
#include <AnimationSinus.h>
#include <AnimationSparkle.h>
#include <AnimationStatic.h>
#include <AnimationStrobe.h>
#include <Clock.h>
#include <WS281X.h>
#include <getopt.h>
#include <stdio.h>

#include <cstring>

WS281X display;

Clock timer(60.0, false);
Animation* currentAnimation = nullptr;
const size_t MAX_ANIMATIONS = 25;
Animation** animations = new Animation*[MAX_ANIMATIONS];

const int CMD_HELP = 'h';
const int CMD_ANIMATION = 'a';
const int CMD_NO_DISPLAY = 'd';

void printHelp() {
    printf("print help info\n");
}

int main(int argc, char** argv) {
    bool disableDisplay = false;
    std::string wantedAnimation = "";

    option long_opts[] = {
        {"animation", required_argument, NULL, CMD_ANIMATION},
        {"no-display", no_argument, NULL, CMD_NO_DISPLAY},
        {"help", no_argument, NULL, CMD_HELP},
        {0}};

    int opt;
    while ((opt = getopt_long_only(argc, argv, "", long_opts, NULL)) != -1) {
        switch (opt) {
            case CMD_HELP:
                printHelp();
                break;
            case CMD_ANIMATION:
                printf("Selected Animation: %s\n", optarg);
                wantedAnimation = optarg;
                break;
            case CMD_NO_DISPLAY:
                disableDisplay = true;
                break;
            case '?':
                printf("Unknown option `-%c'.\n", optopt);
                printHelp();
                break;
            default:
                printf("type base --help for details\n");
        }
    }

    printf("Initializing Display...\n");
    int cols = 25;
    int rows = 25;
    printf("cols: %d rows: %d...\n", cols, rows);
    display.enableDebug(disableDisplay);
    display.init(cols, rows, 0);

    printf("Start animations...\n");
    for (size_t i = 0; i < MAX_ANIMATIONS; i++) {
        animations[i] = nullptr;
    }
    printf("Setting animations...\n");
    printf("AnimationStatic...\n");
    animations[0] = new AnimationStatic();
    printf("AnimationGradient...\n");
    animations[1] = new AnimationGradient();
    printf("AnimationMatrix...\n");
    animations[2] = new AnimationMatrix();
    printf("AnimationScanLine...\n");
    animations[3] = new AnimationScanLine();
    printf("AnimationSparkle...\n");
    animations[4] = new AnimationSparkle();
    printf("AnimationStrobe...\n");
    animations[5] = new AnimationStrobe();
    printf("AnimationSinus...\n");
    animations[6] = new AnimationSinus();

    for (size_t i = 0; i < MAX_ANIMATIONS; i++) {
        if (animations[i] == nullptr) continue;
        animations[i]->init(&display);
    }

    for (size_t i = 0; i < MAX_ANIMATIONS; i++) {
        if (animations[i] == nullptr) continue;
        if (strcmp(wantedAnimation.c_str(), animations[i]->getName()) != 0) continue;
        currentAnimation = animations[i];
        break;
    }

    if (currentAnimation == nullptr) {
        printf("Animation \"%s\" not found\n", wantedAnimation);
        return 1;
    }

    while (true) {
        if (timer.update()) {
            if (currentAnimation != nullptr) {
                currentAnimation->update();
            }
        }
    }

    return 0;
}
