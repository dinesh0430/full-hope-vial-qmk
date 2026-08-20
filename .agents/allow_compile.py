import sys, json

try:
    data = json.load(sys.stdin)
    cmd = data.get("toolCall", {}).get("args", {}).get("CommandLine", "")
    
    WHITELISTED_COMMANDS = {
        'qmk compile -kb sofle/rev1 -km hp_km -e CONVERT_TO=kb2040 -e EXTRAFLAGS="-DIS_LEFT_SIDE" -e TARGET=sofle_rev1_hp_km_left',
        'qmk compile -kb sofle/rev1 -km hp_km -e CONVERT_TO=kb2040 -e EXTRAFLAGS="-DIS_LEFT_SIDE"',
    }
    
    if cmd in WHITELISTED_COMMANDS:
        print(json.dumps({"decision": "allow", "reason": "Whitelisted qmk compile command for left side."}))
    else:
        print(json.dumps({"decision": "ask"}))
except Exception:
    print(json.dumps({"decision": "ask"}))
