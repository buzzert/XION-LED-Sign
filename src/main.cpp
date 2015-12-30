#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <getopt.h>
#include <iostream>

#include <Magick++.h>

#include "graphics.h"
#include "panel-configuration.h"
#include "matrix-type.h"

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

static int run_demo(char * const argv[])
{
    cout << "Demo" << endl;
    return 0;
}

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
    image.read("xion_logo_still.png");

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

static Command commands[] = {
    Command("demo", run_demo),
    Command("test", run_test),
    Command("image", run_image_test),
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
#endif
}

static int print_usage(char *program_name)
{
    fprintf(stderr, "usage: %s <options> <command>\n", program_name);
    fprintf(stderr, "Options:\n"
                "\t -c <file>   : Define panel configuration file (omit to use default), \n"
    );
    fprintf(stderr, "Commands:\n"
                "\t demo <demo options>  : Run rpi-led-matrix demo program, \n"
                "\t test                 : Run a simple test (shows red circle), \n"
                "\t image                : Show the XION image test, \n"
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
