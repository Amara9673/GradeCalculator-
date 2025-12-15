#ifndef THEME_H
#define THEME_H

// theme for the app
const char *THEME =
    // main window
    "window { background: #8aa9ff; font-size: 16px; }"

    // sidebar
    ".sidebar { background: #8aa9ff; padding: 14px; }"
    ".sidebar label { color: #ffffff; font-size: 16px; }"
    ".sidebar button { background: transparent; color: #ffffff; border: 3px solid #cfcfcf; border-radius: 8px; padding: 10px; font-size: 16px; min-height: 40px; }"
    ".sidebar button:hover { background: rgba(255,255,255,0.12); }"
    ".sidebar button:active { background: rgba(255,255,255,0.18); }"

    // content area
    ".content { background: #8aa9ff; padding: 10px; }"
    ".content label { color: #ffffff; font-size: 16px; }"
    ".display-area { background: #ffffff; padding: 12px; border-radius: 10px; }"
    ".display-area textview { background: #ffffff; color: #000000; font-size: 16px; }"

    // helper classes 
    ".mono { font-family: monospace; }"

    // avatar
    ".avatar-frame { border: 3px solid #cfcfcf; border-radius: 8px; }"
    ".avatar-empty { background: #8aa9ff; color: #ffffff; padding: 10px; border-radius: 8px; }"

    // login
    ".login-left { background: #8aa9ff; padding: 34px; }"
    ".login-right { background: #ffffff; padding: 34px; }"
    ".app-title { color: #ffffff; font-size: 34px; font-weight: 700; }"
    ".app-subtitle { color: #ffffff; font-size: 16px; }"
    ".login-title { font-size: 22px; font-weight: 700; }"
    ".login-right label { font-size: 16px; }"
    ".login-right entry { font-size: 16px; padding: 8px; }"
    ".login-btn { background: #4a6cf7; color: #ffffff; border-radius: 8px; padding: 10px; font-size: 16px; min-height: 40px; }"
    ".login-btn:hover { background: #3f61f0; }";

#endif
