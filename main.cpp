#include <windows.h>
#include <iostream>

void set_red_color() {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_INTENSITY);
}

void set_default_color() {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
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
    set_red_color();
    std::cout << "sozari checker\n\n";
    set_default_color();

    int secure_boot_status = is_secure_boot_enabled();
    if (secure_boot_status) {
        std::cout << "secure_boot : enabled\n";
    } else {
        std::cout << "secure_boot : disabled\n";
    }

    int hvci_status = is_hvci_enabled();
    if (hvci_status) {
        std::cout << "hvci : enabled\n";
    } else {
        std::cout << "hvci : disabled\n";
    }

    int tpm_status = is_tpm_enabled();
    if (tpm_status) {
        std::cout << "tpm : enabled\n";
    } else {
        std::cout << "tpm : disabled\n";
    }

    return 0;
}
