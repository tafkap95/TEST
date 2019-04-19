#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <string>
#include <vector>

using namespace std;

vector<string> get_folders_list(const char* im_path);
void GetDesktopResolution(int& horizontal, int& vertical);
void CreatemyToolbar(HWND im_hwnd, HINSTANCE im_inst);
string get_category(string im_str);
void Move_dir(string im_dir);
string search_dest_dir(string im_cat);


void exec_process(void);
void init_process(void);

#endif // FUNCTIONS_H_INCLUDED
