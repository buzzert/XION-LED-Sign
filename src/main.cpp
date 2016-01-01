#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <getopt.h>
#include <iostream>
#include <libgen.h>

#include <Magick++.h>

#include "graphics.h"
#include "panel-configuration.h"
#include "matrix-type.h"

#include "sprite-animation-screen.h"
#include "screen-coordinator.h"
#include "interstitial-screens.h"
#include "text-screen.h"
#include "clock-screen.h"

using namespace rgb_matrix;
using namespace std;

static struct Options {
    string panel_configuration_file;
} gOptions;

/// Shared matrix to be used throughout the process
static Matrix *shared_matrix();

/// Starts running the shared canvas, if applicable. This blocks the calling thread.
static void run_shared_matrix();

#pragma mark Commands

typedef int(*command_function_t)(char * const argv[]);
struct Command {
    Command(string name, command_function_t function)
        : name(name), function(function) {};

    string name;
    command_function_t function;
};

static int run_test(char * const argv[])
{
    Matrix *canvas = shared_matrix();
    DrawCircle(canvas, 5, 5, 5, Color(0xFF, 0x00, 0x00));

    run_shared_matrix();
    return 0;
}

static int run_image_test(char * const argv[])
{
    Magick::Image image;
    image.read("xion_logo_still.gif");

    Canvas *sharedCanvas = shared_matrix();

    for (size_t y = 0; y < image.rows(); ++y) {
        for (size_t x = 0; x < image.columns(); ++x) {
            const Magick::Color &c = image.pixelColor(x, y);
            sharedCanvas->SetPixel(x, y,
                           ScaleQuantumToChar(c.redQuantum()),
                           ScaleQuantumToChar(c.greenQuantum()),
                           ScaleQuantumToChar(c.blueQuantum())
           );
        }
    }

    run_shared_matrix();

    return 0;
}

static int run_text_test(char * const argv[])
{
    Matrix *matrix = shared_matrix();

    Font font;
    if (!font.LoadFont("../lib/rpi-rgb-led-matrix/fonts/4x6.bdf")) {
        cerr << "Error loading font" << endl;
        return 1;
    }

    Color white(255, 255, 255);
    rgb_matrix::DrawText(matrix, font, 0, 0 + font.baseline(), white, "Upcoming events:");

    Font bigFont;
    bigFont.LoadFont("../lib/rpi-rgb-led-matrix/fonts/6x10.bdf");

    Color red(255, 0, 0);
    rgb_matrix::DrawText(matrix, bigFont, 0, font.baseline() + bigFont.baseline() + 1, red, "YOUR MOM");

    run_shared_matrix();

    return 0;
}

static int run_sequence(char * const argv[])
{
    Matrix *m = shared_matrix();

    vector<TickerScreen *> screens;

    SpriteAnimationScreen xionLogo(m, "xion_logo_anim.gif");
    screens.push_back(&xionLogo);

    TextScreen wifiPassword(m, TextScreen::LabelStyle::Subtitle);
    wifiPassword.scrollingStyle = TextScreen::ScrollingStyle::ScrollIn;
    wifiPassword.SetTitleLabel("w0ww0wvenus");
    wifiPassword.titleColor = Color(0xFF, 0x00, 0x00);
    wifiPassword.SetSubtitleLabel("WiFi Password:");
    wifiPassword.subtitleColor = Color(0xFF, 0xFF, 0x00);
    wifiPassword.setDuration(-1);
    screens.push_back(&wifiPassword);

    InterstitialScreen::DDRArrows ddrArrows(m);
    ddrArrows.setDuration(10);
    screens.push_back(&ddrArrows);

    TextScreen messageScreen(m, TextScreen::LabelStyle::Default);
    messageScreen.scrollingStyle = TextScreen::ScrollingStyle::ScrollIn;
    messageScreen.titleColor = Color(0x00, 0xFF, 0x00);
    messageScreen.SetTitleLabel("Why do you need Konami original songs?");
    messageScreen.scrollingSpeed = 0.02;
    messageScreen.setDuration(-1);
    screens.push_back(&messageScreen);

    InterstitialScreen::RainbowRoad rainbowRoad(m);
    screens.push_back(&rainbowRoad);

    ClockScreen clockScreen(m);
    clockScreen.setDuration(8);
    screens.push_back(&clockScreen);

    SpriteAnimationScreen nyanCat(m, "nyan_cat.gif");
    screens.push_back(&nyanCat);

    ScreenCoordinator coordinator(screens);
    coordinator.Start();

    run_shared_matrix();

    return 0;
}

static Command commands[] = {
    Command("test", run_test),
    Command("image", run_image_test),
    Command("run", run_sequence),
    Command("text", run_text_test),
};

#pragma mark -

static void sigint_handler(int sig)
{
    exit(0);
}

static void setup_sighandler()
{
    struct sigaction handler;
    handler.sa_handler = sigint_handler;

    sigemptyset(&handler.sa_mask);
    handler.sa_flags = 0;

    sigaction(SIGINT, &handler, NULL);
}

static Matrix *shared_matrix()
{
    static Matrix *matrix = NULL;
    if (!matrix) {
        PanelConfiguration panelConfig;
        if (gOptions.panel_configuration_file.length()) {
            panelConfig.readConfigurationFile(gOptions.panel_configuration_file);
        }

#ifdef USE_VIRTUAL_CANVAS
        matrix = new VirtualCanvas(panelConfig.rows, panelConfig.chainedDisplays, panelConfig.parallelDisplays);
#else
        GPIO *io = new GPIO();
        if (!io->Init()) {
            cerr << "Unable to initialize GPIO. Permissions?" << endl;
            return nullptr;
        }
        matrix = new RGBMatrix(io, panelConfig.rows, panelConfig.chainedDisplays, panelConfig.parallelDisplays);
#endif
    }

    return matrix;
}

static void run_shared_matrix()
{
#ifdef USE_VIRTUAL_CANVAS
    VirtualCanvas *matrix = shared_matrix();
    matrix->StartSimulation();
#else
    cout << "Press <RETURN> to exit." << endl;
    getchar();
#endif
}

static int print_usage(char *program_name)
{
    fprintf(stderr, "usage: %s <options> <command>\n", program_name);
    fprintf(stderr, "Options:\n"
                "\t -c <file>   : Define panel configuration file (omit to use default), \n"
    );
    fprintf(stderr, "Commands:\n"
                "\t test                 : Run a simple test (shows red circle), \n"
                "\t image                : Show the XION image test, \n"
                "\t run                  : Start running programmed sequence, \n"
                "\t text                 : Show some test text, \n"
    );

    return 1;
}

static int execute_command(const string &command_name, char * const argv[])
{
    command_function_t function = NULL;
    for (unsigned int i = 0; i < (sizeof(commands) / sizeof(Command)); i++) {
        Command c = commands[i];
        if (c.name == command_name) {
            function = c.function;
            break;
        }
    }

    if (function) {
        function({});
    } else {
        cerr << "Command not found: " << command_name << endl;
    }

    return 0;
}

int main(int argc, char * const argv[])
{
    setup_sighandler();
    Magick::InitializeMagick(*argv);

    string command_name;
    char * const program_name = basename(argv[0]);

    int option;
    bool done = false;
    while ( !done && ((option = getopt(argc, argv, "c:h")) != -1) ) {
        switch (option) {
            case 'c':
                gOptions.panel_configuration_file = string(optarg);
                break;
            case 'h':
                return print_usage(program_name);
                break;
            default:
                done = true;
                break;
        }
    }

    if ( (optind < argc) && strlen(argv[optind]) ) {
        command_name.assign(argv[optind]);
    } else {
        print_usage(program_name);
        return 1;
    }

    return execute_command(command_name, {});
}
