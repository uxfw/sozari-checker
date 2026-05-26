#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>

void set_red_color() {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_INTENSITY);
}

void set_green_color() {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console_handle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}

void set_default_color() {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void print_ascii_art() {
    set_red_color();
    std::cout << R"(                                                            
                                                      ██    
                                                      ▀▀    
 ▄▄█████▄   ▄████▄   ████████   ▄█████▄   ██▄████   ████    
 ██▄▄▄▄ ▀  ██▀  ▀██      ▄█▀    ▀ ▄▄▄██   ██▀         ██    
  ▀▀▀▀██▄  ██    ██    ▄█▀     ▄██▀▀▀██   ██          ██    
 █▄▄▄▄▄██  ▀██▄▄██▀  ▄██▄▄▄▄▄  ██▄▄▄███   ██       ▄▄▄██▄▄▄ 
  ▀▀▀▀▀▀     ▀▀▀▀    ▀▀▀▀▀▀▀▀   ▀▀▀▀ ▀▀   ▀▀       ▀▀▀▀▀▀▀▀ 
                                                            
                                                            
)";
    set_default_color();
}

int check_registry_dword(HKEY root_key, const char* sub_key, const char* value_name) {
    HKEY key_handle;
    if (RegOpenKeyExA(root_key, sub_key, 0, KEY_READ, &key_handle) == ERROR_SUCCESS) {
        DWORD data_value;
        DWORD data_size = sizeof(data_value);
        if (RegQueryValueExA(key_handle, value_name, nullptr, nullptr, (LPBYTE)&data_value, &data_size) == ERROR_SUCCESS) {
            RegCloseKey(key_handle);
            return data_value;
        }
        RegCloseKey(key_handle);
    }
    return 0;
}

int is_secure_boot_enabled() {
    return check_registry_dword(HKEY_LOCAL_MACHINE, "system\\currentcontrolset\\control\\secureboot\\state", "uefisecurebootenabled");
}

int is_hvci_enabled() {
    return check_registry_dword(HKEY_LOCAL_MACHINE, "system\\currentcontrolset\\control\\deviceguard\\scenarios\\hypervisorenforcedcodeintegrity", "enabled");
}

int is_tpm_enabled() {
    HKEY key_handle;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "system\\currentcontrolset\\services\\tpm\\wmi", 0, KEY_READ, &key_handle) == ERROR_SUCCESS) {
        DWORD data_value;
        DWORD data_size = sizeof(data_value);
        if (RegQueryValueExA(key_handle, "start", nullptr, nullptr, (LPBYTE)&data_value, &data_size) == ERROR_SUCCESS) {
            RegCloseKey(key_handle);
            if (data_value != 4) {
                return 1;
            }
        }
        RegCloseKey(key_handle);
    }
    return 0;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    system("cls");
    print_ascii_art();

    std::ofstream out_file("sozari results.txt");

    int tpm_status = is_tpm_enabled();
    std::cout << "tpm : ";
    out_file << "tpm : ";
    if (tpm_status) {
        set_green_color();
        std::cout << "enabled\n";
        out_file << "enabled\n";
    } else {
        set_red_color();
        std::cout << "disabled\n";
        out_file << "disabled\n";
    }
    set_default_color();

    int secure_boot_status = is_secure_boot_enabled();
    std::cout << "secure_boot : ";
    out_file << "secure_boot : ";
    if (secure_boot_status) {
        set_green_color();
        std::cout << "enabled\n";
        out_file << "enabled\n";
    } else {
        set_red_color();
        std::cout << "disabled\n";
        out_file << "disabled\n";
    }
    set_default_color();

    int hvci_status = is_hvci_enabled();
    std::cout << "hvci : ";
    out_file << "hvci : ";
    if (hvci_status) {
        set_green_color();
        std::cout << "enabled\n";
        out_file << "enabled\n";
    } else {
        set_red_color();
        std::cout << "disabled\n";
        out_file << "disabled\n";
    }
    set_default_color();

    out_file.close();

    _getwch();
    return 0;
}
