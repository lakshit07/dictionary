#include <src/Dictionary.h>

#include <memory>

int main(int argc, const char** argv) {
    auto dictionary = std::make_unique<Dictionary>();

    if (not dictionary->init(argc, argv)) {
        exit(EXIT_FAILURE);
    }

    dictionary->process();

    return 0;
}
