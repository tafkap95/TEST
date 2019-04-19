#define _WIN32_IE 0x0300
//#define DEBUG
//#define DEBUG_GET_DIR_LIST
#define DEBUG_CATEGORY
//#define DEBUG_DIR_BY_CAT
//#define DEBUG_MOVE_DIR
//#define DEBUG_PATH

#include <dirent.h>
#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <dirent.h>
#include "wtypes.h"
#include <commctrl.h>
#include "resource.h"
#include "functions.h"
#include "main.h"

#include "common.h"

using namespace std;

void exec_process(void)
{
    vector<string> dir_list;
    string dir;

#ifdef DEBUG_PATH
    cout << "Global path:" << g_path << endl;
#endif // DEBUG_PATH

// Read source directories
    dir_list = get_directory_list(g_path);
//    get_repositories(g_path);

// Move directories
    for (vector<string>::iterator t=dir_list.begin(); t!=dir_list.end(); ++t)
    {
        Move_dir(*t);
    }

}

void get_repositories(string im_path)
{
    DIR *dir;
    struct dirent *ent;
    string tmp;
    vector<string> wlt_dir;

    if ((dir = opendir (im_path.c_str())) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            tmp = ent->d_name;

            if ( ent->d_type == DT_DIR )
            {
                if ( ( tmp != "." ) && ( tmp != ".." ) )
                {
                    int i = tmp.size();

                    string str2 = tmp.substr(2,i-2);

                    wlt_dir.push_back(tmp.substr(2,i-2));
                    printf ("%s\n", tmp.c_str());
                }
            }
        }
        closedir (dir);
    }
};

void init_process(void)
{

#ifdef DEBUG
    // Displaying the 2D vector
    for (int i = 0; i < dir_by_cat.size(); i++)
    {
        for (int j = 0; j < dir_by_cat[i].size(); j++)
            cout << dir_by_cat[i][j] << " ";
        cout << endl;
    }
#endif // DEBUG

};

bool check_directory(string im_dir)
{
    std::string str2 = im_dir.substr (0,2);

#ifdef DEBUG
    cout << "\ncheck_directory: " << str2 << endl;
#endif // DEBUG

    if ( str2 == "@@" )
    {
#ifdef DEBUG
        cout << "\ncheck_directory: " << "TRUE" << endl;
#endif // DEBUG
        return true;
    }
    else
    {
#ifdef DEBUG
        cout << "\ncheck_directory: " << "FALSE" << endl;
#endif // DEBUG
        return false;
    }

};

/****************************************************/
/* List all repositories for a path                 */
/****************************************************/
vector<string> get_directory_list(string im_path)
{
    vector<string> vector_dir;
    const char* t_path = im_path.c_str();
    string str_tmp;

    DIR *dir = opendir(t_path);

    struct dirent *entry = readdir(dir);

    while (entry != NULL)
    {
        str_tmp = entry->d_name;

//        {
        if ( ( str_tmp != "." ) && ( str_tmp != ".." ) )
        {
#ifdef DEBUG
            cout << "Directory to check: " << entry->d_name << endl;
#endif // DEBUG

            if (check_directory(str_tmp) == true)
            {
                int i = str_tmp.size();

                string t_str = str_tmp;

                string t_str2 = t_str.substr(2,i-2);
                vector_dir.push_back(t_str2);
            }

        }

        entry = readdir(dir);
    }

    closedir(dir);

#ifdef DEBUG_GET_DIR_LIST
    cout << "Source directories size: " << vector_dir.size() << endl;

// Displaying the 2D vector
    for (vector<string>::iterator t=vector_dir.begin(); t!=vector_dir.end(); ++t)
        cout<<*t<<endl;
#endif // DEBUG

    return vector_dir;
};

// Get the horizontal and vertical screen sizes in pixel
void GetDesktopResolution(int& horizontal, int& vertical)
{
    RECT desktop;
    // Get a handle to the desktop window
    const HWND hDesktop = GetDesktopWindow();
    // Get the size of screen to the variable desktop
    GetWindowRect(hDesktop, &desktop);
    // The top left corner will have coordinates (0,0)
    // and the bottom right corner will have coordinates
    // (horizontal, vertical)
    horizontal = desktop.right;
    vertical = desktop.bottom;
}

void CreatemyToolbar(HWND im_hwnd, HINSTANCE im_inst)
{
    INITCOMMONCONTROLSEX InitCtrlEx ;
    InitCtrlEx.dwSize = sizeof (INITCOMMONCONTROLSEX);
    InitCtrlEx.dwICC = ICC_BAR_CLASSES;
    InitCommonControlsEx( &InitCtrlEx) ;

    TBBUTTON tbrButtons[3];

    tbrButtons[0].iBitmap   = 0;
    tbrButtons[0].idCommand = ID_BTN_START;
    tbrButtons[0].fsState   = TBSTATE_ENABLED;
    tbrButtons[0].fsStyle   = TBSTYLE_BUTTON;
    tbrButtons[0].dwData    = 0L;
    tbrButtons[0].iBitmap   = 0;
    tbrButtons[0].iString   = 0;

    tbrButtons[1].iBitmap   = 1;
    tbrButtons[1].idCommand = ID_BTN_STOP;
    tbrButtons[1].fsState   = TBSTATE_ENABLED;
    tbrButtons[1].fsStyle   = TBSTYLE_BUTTON;
    tbrButtons[1].dwData    = 0L;
    tbrButtons[1].iString   = 0;

    tbrButtons[2].iBitmap   = 2;
    tbrButtons[2].idCommand = ID_BTN_EXIT;
    tbrButtons[2].fsState   = TBSTATE_ENABLED;
    tbrButtons[2].fsStyle   = TBSTYLE_BUTTON;
    tbrButtons[2].dwData    = 0L;
    tbrButtons[2].iString   = 0;

    HWND hWndToolbar;

    hWndToolbar = CreateToolbarEx(im_hwnd,
                                  WS_VISIBLE | WS_CHILD | WS_BORDER,
                                  ID_MYTOOLBAR,
                                  3,
                                  im_inst,
                                  ID_MYTOOLBAR,
                                  tbrButtons,
                                  3,
                                  16, 16, 16, 16,
                                  sizeof(TBBUTTON));

    DWORD error = GetLastError();

}

void Move_dir(string im_dir)
{
    string full_source;
    string full_destination;

    full_source.append(g_path);
    full_source.append("\\\@@");
    full_source.append(im_dir);

    cout << "FULL_SOURCE: " << full_source << endl;

#ifdef DEBUG_MOVE_DIR
    cout << "MOVE_DIR: " << im_dir << endl;
#endif // DEBUG_MOVE_DIR

// Determine category
    string category = get_category(im_dir);

// Determine destination folder name
    string destination_name = get_name(im_dir);

#ifdef DEBUG_CATEGORY
    cout << "debug category: " << category << "\n";
#endif // DEBUG_CATEGORY

// Search destination directory from category
    string dir_destination = search_dest_dir(category);

#ifdef DEBUG_CATEGORY
    cout << "destination Directory: " << dir_destination << "\n";
#endif // DEBUG_CATEGORY

    bool dir_exist = dirExists( dir_destination );

    if ( dir_exist == false )
        create_destination_repository(dir_destination);

    full_destination = dir_destination + "\\" + destination_name;

    cout << "Source:" << full_source.c_str() << endl;
    cout << "Destination: " << dir_destination.c_str() << endl;
    cout << "Full destination: " << full_destination.c_str() << endl;

    if (!MoveFileEx(full_source.c_str(), full_destination.c_str(), MOVEFILE_WRITE_THROUGH))
    {
        DWORD lastError = GetLastError();
        printf ("MoveFileEx failed with error %d\n", GetLastError());
        return;
    }
    else
        printf(TEXT("%s has been moved to %s\n"), full_source.c_str(), dir_destination.c_str());

}

string search_dest_dir(string im_cat)
{

#ifdef DEBUG_DIR_BY_CAT
    for(int i = 0; i < dir_by_cat.size(); ++i)
    {
        cout << "DEBUG DIR_BY_CAT: " << dir_by_cat[i][0] << "       " << dir_by_cat[i][1] << endl;
    }
#endif // DEBUG_DIR_BY_CAT

// Search...
    for(int i = 0; i < dir_by_cat.size(); ++i)
    {
        if ( im_cat == dir_by_cat[i][0] )
        {

#ifdef DEBUG_DIR_BY_CAT
            cout << "Destination found:" << dir_by_cat[i][1] << endl;
#endif

            return dir_by_cat[i][1];

            break;
        }
    }
}

string get_category(string im_str)
{
    string str_cat;
//    bool c_sp = false;

#ifdef DEBUG_CATEGORY
    cout << "STRING: " << im_str << endl;
#endif // DEBUG_CATEGORY

    size_t found = im_str.find("@@");

    str_cat = im_str.substr(0,found);

    return str_cat;
}

string get_name(string im_dir)
{
    string ret;

    size_t m_size = im_dir.size();
    size_t found = im_dir.find("@@");

    ret = im_dir.substr(found+2,m_size-found-2);


    return ret;
}

bool dirExists(const std::string& dirName_in)
{
    DWORD ftyp = GetFileAttributesA(dirName_in.c_str());

    if (ftyp == INVALID_FILE_ATTRIBUTES)
        return false;  //something is wrong with your path!

    if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
        return true;   // this is a directory!

    return false;    // this is not a directory!
}

void create_destination_repository(string im_dir)
{
    bool ret = CreateDirectory( im_dir.c_str(), NULL );

}


void AddControls(HWND hwnd)
{
    HWND hWndList;      // Handle to the list view window
    RECT rcl;           // Rectangle for setting the size of the window

    // Ensure that the common control DLL is loaded.
    InitCommonControls();

    // Get the size and position of the parent window
    GetClientRect(hwnd, &rcl);

    hWndList = CreateWindowEx( 0L,
                               WC_LISTVIEW,                // list view class
                               "",                         // no default text
                               WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT |
                               LVS_EDITLABELS | WS_EX_CLIENTEDGE,
                               0, 0,
                               rcl.right - rcl.left, rcl.bottom - rcl.top,
                               hwnd,
                               (HMENU) ID_LISTVIEW,
                               hInst,
                               NULL );

//   CreateWindowW(L"static", L"Enter text:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,200,100,100,50,hwnd, NULL, NULL, NULL );
//    CreateWindowW(L"Edit", L"Enter text:", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,200,152,100,50,hwnd, NULL, NULL, NULL );

}
