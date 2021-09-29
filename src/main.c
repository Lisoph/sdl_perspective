#include "app.h"

int main(void) {
    App app;

    if (!app_init(&app)) {
        app_destroy(&app);
        return 0;
    }

    app_run(&app);
    app_destroy(&app);
    return 0;
}