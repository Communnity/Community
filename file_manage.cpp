//
// Created by Jiyang Gong on 2019-05-08.
//

#include "file_manage.h"


vector<string> ReadAllFileNameInDirectory(char* filePath ) {
    vector<string> file_name_str;
    struct dirent *dirp;

    DIR* dir = opendir(filePath);

    while ((dirp = readdir(dir)) != nullptr) {
        if (dirp->d_type == DT_REG) {
            // 文件
            file_name_str.push_back(dirp->d_name);

        } else if (dirp->d_type == DT_DIR) {
            // 文件夹
            //file_name_str.push_back(dirp->d_name);
        }
    }
    closedir(dir);
    return file_name_str;
}