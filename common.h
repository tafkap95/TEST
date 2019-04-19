#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

extern vector<vector<string> > dir_by_cat;
extern vector<vector<string> > command_list;
extern vector<string> get_directory_list(string im_path);
extern void get_repositories(string im_path);
extern bool dirExists(const std::string& dirName_in);
extern void AddControls(HWND hwnd);
void create_destination_repository(string im_dir);
extern string get_name(string im_dir);
extern string g_path;

// global current instance
extern HINSTANCE hInst;


typedef struct sData{
    string source;
    string destination;
}sData;

#endif // COMMON_H_INCLUDED
