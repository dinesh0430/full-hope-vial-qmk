import sys, json

try:
    data = json.load(sys.stdin)
    cmd = data.get("toolCall", {}).get("args", {}).get("CommandLine", "")
    
    if cmd == 'qmk compile -kb sofle/rev1 -km hptest -e CONVERT_TO=kb2040 -e EXTRAFLAGS="-DIS_LEFT_SIDE"':
        print(json.dumps({"decision": "allow", "reason": "Whitelisted qmk compile command for left side."}))
    else:
        print(json.dumps({"decision": "ask"}))
except Exception:
    print(json.dumps({"decision": "ask"}))
