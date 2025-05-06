/*
 *
 * CraxFetch - I wanted to do something like neo/fastfetch, as it's been a long time
 * and programming has changed so much since I used to do it - Shane Loy (DayTripper)
 * Find me on IRC: AustNet: #Lismore
 * Email: shane_loy@hotmail.com
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
// Includes live here
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <time.h>
#include <pwd.h>
#include <limits.h> // For HOST_NAME_MAX
#include <sys/types.h>

// Defines live here
#define CRAXFETCH_VERSION "CraxFetch-v0.1-alpha - CraxBank Edition"
#define RED "\033[0;31m"
#define RESET   "\033[0m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define RESET "\033[0m"
#define RUN_COUNT_FILE "/tmp/.craxfetch_count"

const char* moods[] = {
    // Hacker Culture / Matrix / Cyberpunk
    "In God we trust, all others we root.",
    "There is no spoon.",
    "Welcome to the desert of the real.",
    "Neo, sooner or later you're going to realize... there's a difference between knowing the path and walking the path.",
    "I know Kung Fu.",
    "Follow the white rabbit.",
    "Mood: 2600 baud and full of spite.",
    "System mood: Half-caffeinated, fully encrypted.",
    "The cake is a lie, but the shell is real.",
    "Mood: Awaiting the singularity.",
    "Warning: Reality.exe not found.",
    "Booted in 'no gods, no sysadmins' mode.",

    // Big Book of Mischief / Hackers Handbook / Classic Mischief
    "Mood: Running from the feds since '96.",
    "Current status: Building blue boxes out of chewing gum and paperclips.",
    "Tip: Payphones hate 2600 Hz tones.",
    "Mood: One foot in /tmp, the other in a jail cell.",
    "Advice: Never cross the beams. Or the FBI.",
    "Nothing is secure. Especially this.",
    "Today's mischief: Redirecting pizza deliveries via DNS spoofing.",
    "Mood: Port scanned your toaster for fun.",
    "Your IP has been forwarded to Santa.",
    "System alert: Too many l33t packets detected.",

    // Satirical System States
    "Server is vibing on port 1337.",
    "Mood: Grungy but functional.",
    "Motto: Patch nothing, fear everything.",
    "Mood: Passive-aggressive packet loss.",
    "Kernel says: 'Don’t talk to me until I’ve had my coffee.'",
    "Mood: Compiling kernel with malicious intent.",
    "Syslog level: Snarky.",
    "Error 418: Mood is a teapot.",
    "CPU temperature: Running hot from all the bad decisions.",
    "Filesystem status: Slightly haunted.",

    // Vintage IRC / BBS Vibes
    "Mood: +o on #phreakz.",
    "Currently wardialing random suburbs.",
    "Active botnet: 42 eggdrops and a dream.",
    "Mood: /whois'ing your mum.",
    "Quote of the day: 'All your base are belong to us.'",
    "BBS connection stable. ANSI art loading...",
    "Message from root: Stop poking the daemon.",

    // Random Shenanigans
    "Mood: Running Doom on the fridge.",
    "Reality buffer overflow.",
    "Mood: Staring into the void (it's blinking back).",
    "Uptime: ∞ (or lying).",
    "This machine is held together with duct tape and bad life choices.",
    "All systems nominal. Define 'nominal.'",
    "Rooted and looted.",
    "Firewall status: Actively gaslighting you.",
    "Mood: That one scene in Hackers where everything explodes.",
    "This system proudly maintained by paranoia and vim macros.",
};

const char* get_random_tagline() {
    static const char* taglines[] = {
        "CraxBank - Where all good girls come to cry",
        "CraxBank - Unauthorized access encouraged.",
        "CraxBank - No logs. No witnesses.",
        "CraxBank - Trust us, we're unstable.",
        "CraxBank - Punching sysadmins since WinNT.",
        "CraxBank - Brought to you by caffeine and regrets.",
        "CraxBank - Running on spite and printf().",
        "CraxBank - We have no idea what we're doing either.",
        "CraxBank - /dev/null never looked so good.",
        "CraxBank - Deprecated, but still dangerous.",
        "CraxBank - The stack is fine, we swear.",
        "CraxBank - Sponsored by 2600 and late nights.",
        "CraxBank - Bringing phreak back.",
        "CraxBank - This ain't your grandma's fetch tool.",
        "CraxBank - Press F1 for warez.",
        "CraxBank - Less secure than a sticky note.",
        "CraxBank - Where malloc meets chaos.",
        "CraxBank - May contain traces of sarcasm.",
        "CraxBank - Because sudo make me a sandwich.",
        "CraxBank - Cracks so good, you'll call us Bunnings.",
        "CraxBank - Now with 10% more segfaults!",
        "CraxBank - Certified by the Ministry of Bad Decisions.",
        "CraxBank - If you see this, it compiled. Barely.",
        "CraxBank - Your secrets are safe. Ours aren't.",
        "CraxBank - Forkbombed since 1997.",
        "CraxBank - Now legally distinct from malware!",
        "CraxBank - Call now: 1-800-SPOOFED",
        "CraxBank - Best served on EFNet.",
    };

    size_t count = sizeof(taglines) / sizeof(taglines[0]);
    srand(time(NULL));  // Seed randomizer
    return taglines[rand() % count];
}


// Function to execute a command and get its output
char* execute_command(const char* command) {
    char* result = NULL;
    FILE* pipe = popen(command, "r");
    if (pipe) {
        char buffer[128];
        size_t size = 0;

        while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
            size_t len = strlen(buffer);
            char* temp = realloc(result, size + len + 1);
            if (!temp) {
                perror("realloc failed");
                free(result);
                pclose(pipe);
                return NULL;
            }
            result = temp;
            strcpy(result + size, buffer);
            size += len;
        }
        pclose(pipe);
    } else {
        perror("popen failed");
    }
    return result;
}

// Function to read a value from /proc/meminfo
long long read_meminfo(const char* key) {
    FILE* fp = fopen("/proc/meminfo", "r");
    if (fp == NULL) {
        perror("Error opening /proc/meminfo");
        return -1;
    }

    char line[256];
    long long value = -1;

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strncmp(line, key, strlen(key)) == 0) {
            if (sscanf(line, "%*s %lld kB", &value) == 1) {
                break;
            }
        }
    }
    fclose(fp);
    return value;
}

char* get_de_info() {
    FILE* fp;
    char output[256];

    if (system("pgrep -x cinnamon > /dev/null 2>&1") == 0) {
        fp = popen("cinnamon --version", "r");
        if (fp && fgets(output, sizeof(output), fp)) {
            strtok(output, "\n");
            pclose(fp);
            return strdup(output); // Cinnamon 5.6.8
        }
    }
    // Add other DEs (gnome-shell, plasma, etc)

    return "Unknown";
}

char* get_terminal_info() {
    static char terminal[256];
    pid_t pid = getppid();
    const char* shells[] = { "bash", "zsh", "fish", "dash", "sh", "ksh", NULL };

    for (int i = 0; i < 6; ++i) {  // Go up to 6 levels just in case
        char cmd_path[64];
        snprintf(cmd_path, sizeof(cmd_path), "/proc/%d/comm", pid);
        FILE* fp = fopen(cmd_path, "r");
        if (fp) {
            if (fgets(terminal, sizeof(terminal), fp)) {
                strtok(terminal, "\n");  // Remove newline
                fclose(fp);

                // Check if it's just a shell (we want to skip these)
                int is_shell = 0;
                for (int j = 0; shells[j] != NULL; ++j) {
                    if (strcmp(terminal, shells[j]) == 0) {
                        is_shell = 1;
                        break;
                    }
                }

                if (!is_shell) {
                    // Map common terminals to pretty names
                    if (strcmp(terminal, "gnome-terminal-") == 0 || strcmp(terminal, "gnome-terminal-server") == 0)
                        return "GNOME Terminal";
                    if (strcmp(terminal, "xfce4-terminal") == 0)
                        return "XFCE4 Terminal";
                    if (strcmp(terminal, "konsole") == 0)
                        return "Konsole";
                    if (strcmp(terminal, "tilix") == 0)
                        return "Tilix";
                    if (strcmp(terminal, "alacritty") == 0)
                        return "Alacritty";
                    if (strcmp(terminal, "xterm") == 0)
                        return "Xterm";
                    if (strcmp(terminal, "kitty") == 0)
                        return "Kitty";
                    if (strcmp(terminal, "wezterm") == 0)
                        return "WezTerm";

                    return terminal;  // Return raw name if unknown
                }
            } else {
                fclose(fp);
            }
        }

        // Get next parent PID
        char status_path[64];
        snprintf(status_path, sizeof(status_path), "/proc/%d/status", pid);
        fp = fopen(status_path, "r");
        if (fp) {
            char line[256];
            while (fgets(line, sizeof(line), fp)) {
                if (strncmp(line, "PPid:", 5) == 0) {
                    pid = atoi(line + 5);
                    break;
                }
            }
            fclose(fp);
        } else {
            break;
        }
    }

    // Fallback
    const char* term_env = getenv("TERM");
    if (term_env) {
        snprintf(terminal, sizeof(terminal), "%s", term_env);
        return terminal;
    }

    return "Unknown";
}



char* get_shell_info() {
    static char shell_info[128];
    const char* shell_path = getenv("SHELL");

    if (!shell_path)
        shell_path = "unknown";

    // Extract just the shell name (e.g., "bash" from "/bin/bash")
    const char* shell_name = strrchr(shell_path, '/');
    shell_name = shell_name ? shell_name + 1 : shell_path;

    // Run `<shell> --version`
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "%s --version", shell_path);

    FILE* fp = popen(cmd, "r");
    if (!fp) {
        snprintf(shell_info, sizeof(shell_info), "%s", shell_name);
        return shell_info;
    }

    char buffer[256];
    if (fgets(buffer, sizeof(buffer), fp)) {
        // Example: "GNU bash, version 5.2.15(1)-release (x86_64-pc-linux-gnu)"
        // Extract just: "bash 5.2.15"
        char* ver = strstr(buffer, "version ");
        if (ver) {
            ver += 8; // Skip "version "
            char* space = strchr(ver, '(');
            if (space) *space = '\0';

            snprintf(shell_info, sizeof(shell_info), "%s %s", shell_name, ver);
        } else {
            snprintf(shell_info, sizeof(shell_info), "%s", shell_name);
        }
    } else {
        snprintf(shell_info, sizeof(shell_info), "%s", shell_name);
    }

    pclose(fp);
    return shell_info;
}


// Function to get CPU model name from /proc/cpuinfo
char* get_cpu_model() {
    FILE* fp = fopen("/proc/cpuinfo", "r");
    if (fp == NULL) {
        perror("Error opening /proc/cpuinfo");
        return NULL;
    }

    char line[256];
    char* model_name = NULL;
    size_t len = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strncmp(line, "model name", 10) == 0) {
            char* colon = strchr(line, ':');
            if (colon) {
                colon += 2; // Move past the colon and space
                // Trim leading and trailing whitespace
                while (*colon == ' ' || *colon == '\t') {
                    colon++;
                }
                size_t model_len = strlen(colon);
                while (model_len > 0 && (colon[model_len - 1] == ' ' || colon[model_len - 1] == '\t' || colon[model_len - 1] == '\n')) {
                    model_len--;
                }
                model_name = strndup(colon, model_len);
                break;
            }
        }
    }
    fclose(fp);
    return model_name;
}

// Function to get the number of CPU cores
int get_cpu_cores() {
    FILE* fp = fopen("/proc/cpuinfo", "r");
    if (fp == NULL) {
        perror("Error opening /proc/cpuinfo");
        return -1;
    }

    int core_count = 0;
    char line[256];

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strncmp(line, "processor", 9) == 0) {
            core_count++;
        }
    }
    fclose(fp);
    return core_count;
}

int get_run_count() {
    const char* home = getenv("HOME");
    char path[512];
    snprintf(path, sizeof(path), "%s/.cf_run_count", home);

    FILE* file = fopen(path, "r+");
    int count = 0;

    if (file) {
        fscanf(file, "%d", &count);
        rewind(file);
    } else {
        file = fopen(path, "w");
    }

    count++;
    fprintf(file, "%d\n", count);
    fclose(file);

    return count;
}

// Moved the entire print_os_info function definition OUTSIDE of main()
void print_os_info() {
    FILE *f = fopen("/etc/os-release", "r");
    char line[256];
    char distro[128] = "Unknown";
    char version[128] = "";

    if (f) {
        while (fgets(line, sizeof(line), f)) {
            if (strncmp(line, "NAME=", 5) == 0) {
                char *name = line + 5;
                name[strcspn(name, "\n")] = 0;
                if (*name == '"' || *name == '\'') {
                    name++;
                    name[strlen(name) - 1] = 0;
                }
                strncpy(distro, name, sizeof(distro));
            }
            if (strncmp(line, "VERSION_ID=", 11) == 0) {
                char *ver = line + 11;
                ver[strcspn(ver, "\n")] = 0;
                if (*ver == '"' || *ver == '\'') {
                    ver++;
                    ver[strlen(ver) - 1] = 0;
                }
                strncpy(version, ver, sizeof(version));
            }
        }
        fclose(f);
    }

    // Add spice here
    const char *emoji = "💻"; // Default emoji
    const char *nickname = distro; // Default nickname

    if (strstr(distro, "Arch")) {
        emoji = "🎯";
        nickname = "Arch Linux (ricing required)";
    } else if (strstr(distro, "Debian")) {
        emoji = "🧱";
        nickname = "Debian (solid as a rock)";
    } else if (strstr(distro, "Ubuntu")) {
        emoji = "🟣";
        nickname = "Ubuntu (the friendly fork)";
    } else if (strstr(distro, "Fedora")) {
        emoji = "🎩";
        nickname = "Fedora (classy & bleeding edge)";
    } else if (strstr(distro, "Red Hat")) {
        emoji = "🔴";
        nickname = "Red Hat (enterprisey as hell)";
    } else if (strstr(distro, "Gentoo")) {
        emoji = "🧬";
        nickname = "Gentoo (compile 'til you die)";
    } else if (strstr(distro, "Void")) {
        emoji = "🕳️";
        nickname = "Void (edge lords only)";
    } else if (strstr(distro, "Slackware")) {
        emoji = "📼";
        nickname = "Slackware (old school kung fu)";
    } else if (strstr(distro, "openSUSE")) {
        emoji = "🦎";
        nickname = "openSUSE (green mean machine)";
    }

    struct utsname uname_data;
    if (uname(&uname_data) == 0) {
        printf("Operating System          : %s %s %s (%s %s)\n",
               emoji, nickname, version,
               uname_data.sysname, uname_data.release);
    } else {
        perror("uname failed");
        printf("Operating System          : %s %s %s\n", emoji, nickname, version);
    }
}
int main(int argc, char *argv[]) {

    // Handle command-line flags
    if (argc > 1) {
        if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0) {
            printf("%s\nBuilt with love by UCF veterans. Uptime is a lie.\n", CRAXFETCH_VERSION);
            return 0;
        }
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            printf("Usage: craxfetch [options]\n");
            printf("  -v, --version    Show version info\n");
            printf("  -h, --help       Show this help message\n");
            printf("  Run without flags to display system info and retro shenanigans.\n");
	    printf("\n  Feeling -l337, --l337 ? Don't run that command ;)\n");
            return 0;
        }
    }
if (argc > 1 && (strcmp(argv[1], "--1337") == 0 || strcmp(argv[1], "-1337") == 0)) {
    const char* bbom_quotes[] = {
        "“Ignorance is curable, stupid is forever.” – BBoM",
        "“Accessing data like it's 1999.”",
        "“To err is human. To really foul things up requires a root password.”",
        "“This is your sysadmin speaking. I have no idea what I'm doing.”",
        "“CraxBank: Because reality is read-only.”",
        "“Don't grep where you eat.”",
        "“Hack the planet!” – The Hackers Handbook"
    };

    srand(time(NULL));
    int q = rand() % (sizeof(bbom_quotes) / sizeof(bbom_quotes[0]));

    printf("\n💀 CraxFetch // 1337 MODE 💀\n");
    printf("Big Book Quote           : %s\n", bbom_quotes[q]);
    printf("Tracing route to localhost [127.0.0.1]...\n\n");
    printf("  1    <1 ms    <1 ms    <1 ms  craxrouter.local\n");
    printf("  2    1337 ms  666 ms  404 ms  middleman.crax [10.66.6.6]\n");
    printf("  3    *        *       *       Request timed out.\n");
    printf("  4    0 ms     1 ms    0 ms    root@localhost\n\n");
    printf("Connection stabilized. Logging into CraxBank mainframe...\n");

    // Fake prompt
    printf("\nroot@CraxBank:~# ACCESS GRANTED\n");
    printf("Welcome back, operative.\n");
    printf("Uplink status: 🔒 Encrypted\n");
    printf("Proceed with discretion...\n\n");

    return 0;
}

    struct utsname uname_data;
    struct passwd *pw;
    char hostname[HOST_NAME_MAX + 1];
    gethostname(hostname, HOST_NAME_MAX);

    // Red ASCII Art
 printf(RED"\n     ▄████▄   ██▀███   ▄▄▄      ▒██   ██▒ ▄▄▄▄    ▄▄▄       ███▄    █  ██ ▄█▀ \n"
           "    ▒██▀ ▀█  ▓██ ▒ ██▒▒████▄    ▒▒ █ █ ▒░▓█████▄ ▒████▄     ██ ▀█   █  ██▄█▒  \n"
           "    ▒▓█    ▄ ▓██ ░▄█ ▒▒██  ▀█▄  ░░  █   ░▒██▒ ▄██▒██  ▀█▄  ▓██  ▀█ ██▒▓███▄░  \n"
           "    ▒▓▓▄ ▄██▒▒██▀▀█▄  ░██▄▄▄▄██  ░ █ █ ▒ ▒██░█▀  ░██▄▄▄▄██ ▓██▒  ▐▌██▒▓██ █▄  \n"
           "    ▒ ▓███▀ ░░██▓ ▒██▒ ▓█   ▓██▒▒██▒ ▒██▒░▓█  ▀█▓ ▓█   ▓██▒▒██░   ▓██░▒██▒ █▄ \n"
           "    ░ ░▒ ▒  ░░ ▒▓ ░▒▓░ ▒▒   ▓▒█░▒▒ ░ ░▓ ░░▒▓███▀▒ ▒▒   ▓▒█░░ ▒░   ▒ ▒ ▒ ▒▒ ▓▒ \n"
           "      ░  ▒     ░▒ ░ ▒░  ▒   ▒▒ ░░░   ░▒ ░▒░▒   ░   ▒   ▒▒ ░░ ░░   ░ ▒░░ ░▒ ▒░ \n"
           "    ░          ░░   ░   ░   ▒    ░    ░   ░    ░   ░   ▒      ░   ░ ░ ░ ░░ ░  \n"
           "    ░ ░         ░           ░  ░ ░    ░   ░            ░  ░         ░ ░  ░    \n"
           "    ░                                          ░                              \n\n");
    printf("                " CYAN "%s" RESET "\n\n", get_random_tagline());

    // User Logged in @ Hostname
    pw = getpwuid(getuid());
    if (pw) {
        printf("User Logged in            : %s@%s\n\n", pw->pw_name, hostname);
    } else {
        perror("getpwuid failed");
        printf("User Logged in            : Unknown@%s\n\n", hostname);
    }

    // Operating System with Distro
    print_os_info(); // Call the function here

    // Simplified Linux Kernel
    char* kernel_info = execute_command("uname -r");
    if (kernel_info) {
        // Trim the newline character
        size_t len = strlen(kernel_info);
        if (len > 0 && kernel_info[len - 1] == '\n') {
            kernel_info[len - 1] = '\0';
        }
        printf("Linux Kernel              : %s\n", kernel_info);
        free(kernel_info);
    } else {
        printf("Linux Kernel              : Unknown\n");
    }

    // Uptime
    char* uptime = execute_command("uptime -p");
    if (uptime) {
        // Remove the "up " prefix
        char* trimmed_uptime = uptime + 3;
        // Remove the trailing newline character if present
        size_t len = strlen(trimmed_uptime);
        if (len > 0 && trimmed_uptime[len - 1] == '\n') {
            trimmed_uptime[len - 1] = '\0';
        }
        printf("Uptime                    : %s\n", trimmed_uptime);
        free(uptime);
    } else {
        printf("Uptime                    : Unknown\n");
    }

/* Start new code */

        printf("Shell                     : %s\n", get_shell_info());
        printf("Desktop Envirnment        : %s\n", get_de_info());
        printf("Terminal                  : %s\n", get_terminal_info());

/* End new code */

    // CPU Information
    char* cpu_model = get_cpu_model();
    int cpu_cores = get_cpu_cores();

    if (cpu_model) {
        printf("CPU                       : %s", cpu_model);
        if (cpu_cores > 0) {
            printf(" (%d Cores)\n", cpu_cores);
        } else {
            printf(" (Cores: Unknown)\n");
        }
        free(cpu_model);
    } else {
        printf("CPU                       : Unknown\n");
    }

// RAM Information
long long mem_total_kb = read_meminfo("MemTotal:");
long long mem_available_kb = read_meminfo("MemAvailable:");

if (mem_total_kb > 0 && mem_available_kb >= 0) {
    long long mem_used_kb = mem_total_kb - mem_available_kb;
    double mem_total_gb = (double)mem_total_kb / (1024 * 1024);
    double mem_used_gb = (double)mem_used_kb / (1024 * 1024);
    double used_percent = (double)mem_used_kb / mem_total_kb * 100.0;

    printf("RAM                       : %.2f GiB / %.2f GiB (%.1f%%)\n",
           mem_used_gb, mem_total_gb, used_percent);
} else {
    printf("RAM                       : Unknown\n");
}



       printf("Motto of the Day          : %s\n", moods[rand() % 5]);
int runs = get_run_count();
if (runs % 7 == 0) {
    const char* easter_eggs[] = {
        "You’ve unlocked 7th layer madness.",
        "Welcome back, elite one. Try not to get caught this time.",
        "You've reached level 7: Root or be rooted.",
        "🎉 Congrats: You've just triggered the fail-safe payload. Just kidding. (Or am I?)",
        "You've dialed into the ghost of Bell Labs.",
        "Easter Egg: This machine self-destructs in 5... 4... psych.",
        "You are now being watched. Again.",
        "The packet is in the pineapple. Repeat: the packet is in the pineapple.",
        "🔥 Access granted to /dev/havoc — proceed with caution.",
        "7th run unlocked: You now have access to the forbidden man pages.",
        "You've reached a hidden node on the darknet IRC ring. Welcome, operator.",
        "Achievement Unlocked: Typing faster than your firewall.",
        "🔓 You cracked the code. Now solve the enigma of your life choices.",
        "There is no source. Only Zuul.",
        "System Mood: 'I know what you did with that fork bomb.'",
    };

    // Pick one at random
    srand(time(NULL)); // seed RNG
    int index = rand() % (sizeof(easter_eggs) / sizeof(easter_eggs[0]));
    printf("Easter Egg                : \"%s\"\n", easter_eggs[index]);
}


    return 0;
}
