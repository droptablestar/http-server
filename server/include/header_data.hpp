const char *weekday[] = { "Sunday", "Monday", "Tuesday", "Wednesday",
                          "Thursday", "Friday", "Saturday"};
const char *months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                         "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

#if defined( __APPLE__)
std::string OS = "Mac OS X";

#elif defined(__unix__)
std::string OS = "Unix";

#elif defined(__hpux)
std::string OS = "HP-UX";

#elif defined(__linux__)
std::string OS = "linux";

#elif defined(_WIN64)
std::string OS = "64-bit Windows (Ewwww)";

#elif defined(_WIN32)
std::string OS = "32-bit Windows (BIGGER EWWWWWW)";

#else
std::string OS = "Undetermined";

#endif
