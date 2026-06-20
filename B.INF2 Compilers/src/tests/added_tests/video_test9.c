int missing_return_path(int x) {
    if (x > 0) {
        return 1;
    }
    // Hier ontbreekt een return!
}

int main() {
    missing_return_path(5);

    return 0;
}
