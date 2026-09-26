import sys, json

try:
    data = json.load(sys.stdin)
    cmd = data.get("toolCall", {}).get("args", {}).get("CommandLine", "")
    
    WHITELISTED_COMMANDS = {
        'qmk compile -kb sofle/rev1 -km hp_km -e CONVERT_TO=kb2040 -e EXTRAFLAGS="-DIS_LEFT_SIDE" -e TARGET=sofle_rev1_hp_km_left',
        'qmk compile -kb sofle/rev1 -km hp_km -e CONVERT_TO=kb2040 -e EXTRAFLAGS="-DIS_LEFT_SIDE"',
        'qmk compile -kb sofle/rev1 -km hp_km -e CONVERT_TO=kb2040 -e EXTRAFLAGS="-DIS_RIGHT_SIDE" -e TARGET=sofle_rev1_hp_km_right',
        'qmk compile -kb sofle/rev1 -km hp_km -e CONVERT_TO=kb2040 -e EXTRAFLAGS="-DIS_RIGHT_SIDE"',
        'python3 keyboards/sofle/rev1/keymaps/hp_km/generate_geometric_anims.py',
        'python3 keyboards/sofle/rev1/keymaps/hp_km/generate_fire_options.py',
        'python3 keyboards/sofle/rev1/keymaps/hp_km/convert_screensavers.py',
        'python3 keyboards/sofle/rev1/keymaps/hp_km/generate_liquid_screensavers.py',
        'python3 keyboards/sofle/rev1/keymaps/hp_km/generate_wind_screensavers.py',
        'python3 keyboards/sofle/rev1/keymaps/hp_km/analyze_new_resources.py',
        'python3 keyboards/sofle/rev1/keymaps/hp_km/generate_sample_pack_previews.py',
    }
    
    if cmd in WHITELISTED_COMMANDS:
        print(json.dumps({"decision": "allow", "reason": "Whitelisted qmk compile command for left side."}))
    else:
        print(json.dumps({"decision": "ask"}))
except Exception:
    print(json.dumps({"decision": "ask"}))
