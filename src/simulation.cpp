#include <AnimationGradient.h>
#include <AnimationMatrix.h>
#include <AnimationScanLine.h>
#include <AnimationSinus.h>
#include <AnimationSparkle.h>
#include <AnimationStatic.h>
#include <AnimationStrobe.h>
#include <Clock.h>
#include <WS281X.h>
#include <stdio.h>

#include <cstring>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/loop.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>
#include <memory>
#include <stdexcept>
#include <string>

ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::Fullscreen();

WS281X display;

Clock timer(60.0, false);
size_t currentAnimationIndex = -1;
Animation* currentAnimation = nullptr;
const size_t MAX_ANIMATIONS = 25;
Animation** animations = new Animation*[MAX_ANIMATIONS];

ftxui::Elements ledmatrix;
std::vector<std::string> debugPanel;

template <typename... Args>
std::string string_format(const std::string& format, Args... args) {
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;  // Extra space for '\0'
    if (size_s <= 0) {
        throw std::runtime_error("Error during formatting.");
    }
    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1);  // We don't want the '\0' inside
}

ftxui::Elements AddFocusBottom(ftxui::Elements list) {
    if (list.size() != 0)
        list.back() = ftxui::focus(std::move(list.back()));
    return std::move(list);
}

int main(int argc, char** argv) {
    debugPanel.push_back("Initializing Display...");
    int cols = 25;
    int rows = 25;
    debugPanel.push_back(string_format("cols: %d rows: %d", cols, rows));
    display.init(cols, rows, 0);

    debugPanel.push_back("Start animations...");
    for (size_t i = 0; i < MAX_ANIMATIONS; i++) {
        animations[i] = nullptr;
    }
    debugPanel.push_back("Setting animations...");
    debugPanel.push_back("AnimationStatic");
    animations[0] = new AnimationStatic();
    debugPanel.push_back("AnimationGradient");
    animations[1] = new AnimationGradient();
    debugPanel.push_back("AnimationMatrix");
    animations[2] = new AnimationMatrix();
    debugPanel.push_back("AnimationScanLine");
    animations[3] = new AnimationScanLine();
    debugPanel.push_back("AnimationSparkle");
    animations[4] = new AnimationSparkle();
    debugPanel.push_back("AnimationStrobe");
    animations[5] = new AnimationStrobe();
    debugPanel.push_back("AnimationSinus");
    animations[6] = new AnimationSinus();

    std::vector<std::string> animationsEntries;

    for (size_t i = 0; i < MAX_ANIMATIONS; i++) {
        if (animations[i] == nullptr) continue;
        animations[i]->init(&display);
        animationsEntries.push_back(animations[i]->getName());
    }

    currentAnimationIndex = 0;
    currentAnimation = animations[currentAnimationIndex];

    auto menuOption = ftxui::MenuOption();
    int animationsEntrySelected = currentAnimationIndex;
    ftxui::Component animationsMenu = ftxui::Menu(&animationsEntries, &animationsEntrySelected, &menuOption);

    float newTPS = timer.getTPS();
    long newR = (display.getStaticColor() >> 16) & 0xFF;
    long newG = (display.getStaticColor() >> 8) & 0xFF;
    long newB = display.getStaticColor() & 0xFF;
    auto sliders = ftxui::Container::Vertical({
        ftxui::Slider("TPS:", &newTPS, 0.0f, 256.0f, 1.0f),
        ftxui::Slider("R:", &newR, 0l, 255l, 1l),
        ftxui::Slider("G:", &newG, 0l, 255l, 1l),
        ftxui::Slider("B:", &newB, 0l, 255l, 1l),
    });

    display.getStrip()->cb = [](ftxui::Elements array) {
        ledmatrix = array;
    };

    auto container = ftxui::Container::Horizontal({
        sliders,
        animationsMenu,
    });

    auto component = ftxui::Renderer(container, [&] {
        ftxui::Elements children;
        for (size_t i = std::max(0, (int)debugPanel.size() - 200); i < debugPanel.size(); ++i)
            children.push_back(ftxui::text(debugPanel[i]));
        std::reverse(children.begin(), children.end());

        return
            // -------- Top panel --------------
            ftxui::hbox({
                // -------- Left Menu --------------
                ftxui::vbox({
                    ftxui::hcenter(ftxui::text("Animations")),
                    ftxui::separator(),
                    animationsMenu->Render(),
                }),
                ftxui::separator(),
                // -------- Right Menu --------------
                ftxui::vbox({
                    ftxui::hcenter(ftxui::text("Settings")),
                    ftxui::separator(),
                    sliders->Render(),
                }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 25),
                ftxui::separator(),
                ftxui::vbox({
                    ftxui::hcenter(ftxui::text("LED Matrix")),
                    ftxui::separator(),
                    ftxui::vbox(std::move(ledmatrix)),
                }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 55),
                ftxui::separator(),
                ftxui::vbox({
                    ftxui::filler(),
                    ftxui::vbox(AddFocusBottom(children)),
                }),
            }) |
            ftxui::xflex | ftxui::border;
    });

    ftxui::Loop loop(&screen, component);

    while (!loop.HasQuitted()) {
        if (timer.update()) {
            if (currentAnimation != nullptr) {
                if (currentAnimation->update()) screen.Post(ftxui::Event::Custom);
            }
        }
        if (timer.getTPS() != newTPS) timer.setTPS(newTPS);

        if (newR != (display.getStaticColor() >> 16) & 0xFF)
            display.setStaticColor(display.getColor(newR, (display.getStaticColor() >> 8) & 0xFF, display.getStaticColor() & 0xFF));

        if (newG != (display.getStaticColor() >> 8) & 0xFF)
            display.setStaticColor(display.getColor((display.getStaticColor() >> 16) & 0xFF, newG, display.getStaticColor() & 0xFF));

        if (newB != display.getStaticColor() & 0xFF)
            display.setStaticColor(display.getColor((display.getStaticColor() >> 16) & 0xFF, (display.getStaticColor() >> 8) & 0xFF, newB));

        if (animationsEntrySelected != currentAnimationIndex) {
            display.clear(true);
            currentAnimationIndex = animationsEntrySelected;
            currentAnimation = animations[currentAnimationIndex];
            debugPanel.push_back(string_format("Updating Animation to %s", currentAnimation->getName()));
        }
        loop.RunOnce();
    }

    return 0;
}
