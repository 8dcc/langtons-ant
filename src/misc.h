void die(char* s) {
    fprintf(stderr, s);

    SDL_Quit();

    exit(1);
}
