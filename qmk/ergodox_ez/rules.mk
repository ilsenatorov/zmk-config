# Oryx/Keymapp support comes from keymap.json's "modules" list on this
# firmware branch (Oryx is a QMK community module here), NOT from an
# ORYX_ENABLE flag — that flag doesn't exist on firmware25 and is ignored.

CONSOLE_ENABLE     = no
COMMAND_ENABLE     = no
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE   = no
MAGIC_ENABLE       = no
MUSIC_ENABLE       = no
LTO_ENABLE         = yes

MOUSEKEY_ENABLE    = no  # mouse layer dropped; frees flash for combos/chordal hold
COMBO_ENABLE       = yes # caps word, underscore, home, end
CAPS_WORD_ENABLE   = yes # target of the F+J combo
EXTRAKEY_ENABLE    = yes # media/volume/brightness keys on ADJ
