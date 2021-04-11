#include "dockerplugin.h"
int main(int argc, char** argv) {
    secplugs::dockerplugin plugin("data/valid.json");
    return plugin.run(argc, argv);
}
