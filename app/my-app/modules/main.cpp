#include <iostream>
#include <dirent.h>

int main() {
    DIR *dir;
    struct dirent *entry;
    const char *path = "../my-app"; // Current directory

    dir = opendir(path); // Open the directory
    if (dir != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            printf("%s\n", entry->d_name); // Print file/folder name
        }
        closedir(dir); // Close the directory
    } else {
        perror("Error opening directory");
    }
    return 0;
}