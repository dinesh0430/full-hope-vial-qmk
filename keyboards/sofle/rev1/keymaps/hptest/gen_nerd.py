import subprocess
import glob

fonts = glob.glob("/usr/share/fonts/TTF/*NerdFont*.ttf")
if not fonts:
    print("No nerd fonts found!")
    exit(1)
font_path = fonts[0]
for f in fonts:
    if "ExtraBold" in f or "Heavy" in f:
        font_path = f
        break

# The requested icons (16 old + 32 new)
codepoints = [
    0xF0973,   # 󰥳
    0xF1FA,    # 
    0xF05C0,   # 󰗀
    0xF0582,   # 󰖂
    0xF0721,   # 󰜡
    0xF0519,   # 󰔙
    0xF03D2,   # 󰏒
    0xF1105,   # 󱄅
    0xF02A2,   # 󰊢
    0xF0684,   # 󰚄
    0xEE82,    # 
    0xEE7A,    # 
    0xED43,    # 
    0xEFA7,    # 
    0xF4BC,    # 
    0xF428,    # 
    0xF0346,   # 󰍆
    0xF0303,   # 󰌃
    0xF02DF,   # 󰋟
    0xF124F,   # 󱉏
    0xF15C6,   # 󱗆
    0xF1183,   # 󱆃
    0xF08C7,   # 󰣇
    0xF1E9,    # 
    0xEF6D,    # 
    0xF06C,    # 
    0xEF56,    # 
    0xF2D6,    # 
    0xF282,    # 
    0xF0E7,    # 
    0xEE99,    # 
    0xE782,    # 
    0xE7C5,    # 
    0xE893,    # 
    0xE6A4,    # 
    0xE620,    # 
    0xE007,    # 
    0xF0451,   # 󰑑
    0xF0D7C,   # 󰵼
    0xF1510,   # 󱔐
    0xF0D72,   # 󰵲
    0xE823,    # 
    0xE806,    # 
    0xE7FA,    # 
    0xE66C,    # 
    0xE666,    # 
    0xF490,    # 
    0xF0CD7,   # 󰳗
    0xF111B,   # 󱄛
    0xF0A08,   # 󰨈
    0xF0BAA,   # 󰮪
    0xF01E4,   # 󰇤
    0xF0816,   # 󰠖
    0xE214,    # 
    0xE26E,    # 
    0xEF10,    # 
    0xEEB0,    # 
    0xF135,    # 
    0xED1B,    # 
    0xF1AA,    # 
    0xF296,    # 
    0xF299,    # 
    0xF1A9,    # 
    0xEEF8,    # 
    0xE894,    # 
    0xE80D,    # 
    0xF1A82,   # 󱪂
    0xF1545,   # 󱕅
    0xF0421,   # 󰐡
    0xF117E,   # 󱅾
    0xF08E0,   # 󰣠
    0xEE83,    # 
    0xF1DF,    # 
    0xEEC9,    # 
    0xEDD5,    # 
    0xF17B,    # 
    0xE8DA,    # 
    0xE8D9,    # 
    0xE89F,    # 
    0xE87F,    # 
    0xE873,    # 
    0xE86E,    # 
    0xE7A1,    # 
    0xE872,    # 
    0xE867,    # 
    0xE7A5,    # 
    0xE82A,    # 
    0xE7D2,    # 
    0xE6B5,    # 
    0xEB73,    # 
    0xE661,    # 
    0xF137F,   # 󱍿
    0xF0967,   # 󰥧
    0xF0204,   # 󰈄
    0xF086A,   # 󰡪
    0xF0674,   # 󰙴
    0xE284,    # 
    0xEDA7,    # 
    0xEF3E,    # 
    0xEE28,    # 
    0xF2D5,    # 
    0xF069,    # 
    0xE805,    # 
    0xE7F4,    # 
    0xE7D7,    # 
    0xE716,    # 
    0xEC10,    # 
    0xF4A1,    # 
    0xF0064,   # 󰁤
    0xF337,    # 
    0xEF35,    # 
    0xF209,    # 
    0xE8A5,    # 
    0xE825,    # 
    0xE801,    # 
    0xE79F,    # 
    0xE752,    # 
    0xE701,    # 
    0xE370,    # 
    0xF366,    # 
    0xE28E,    # 
    0xED28,    # 
    0xF230,    # 
    0xE72A,    # 
    0xF344,    # 
    0xF0A3,    # 
    0xF31D,    # 
    0xED24,    # 
    0xF0CD1,   # 󰳑
    0xF047A,   # 󰑺
    0xF04C8,   # 󰓈
    0xF2DC,    # 
    0xF1950,   # 󱥐
    0xF0907,   # 󰤇
    0xEEE6,    # 
    0xF197,    # 
    0xEF50,    # 
    0xF4F5,    # 
    0xF0EA4,   # 󰺤
    0xF156B,   # 󱕫
    0xF51B,    # 
    0xF0787,   # 󰞇
    0xF0BC9,   # 󰯉
    0xE721,    # 
    0xF15C3,   # 󱗃
    0xF0717,   # 󰜗
    0xF0DF1,   # 󰷱
    0xF0B62,   # 󰭢
    0xEDEC,    # 
    0xEFA0,    # 
    0xF0383,   # 󰎃
    0xF1303,   # 󱌃
    0xEED5,    # 
    0xF0CED,   # 󰳭
    0xF02C4,   # 󰋄
    0xF0771,   # 󰝱
    0xF07E1,   # 󰟡
    0xEDA0,    # 
    0xF27F,    # 
    0xF0C93,   # 󰲓
    0xF0B90,   # 󰮐
    0xF0857,   # 󰡗
    0xF0433,   # 󰐳
    0xF0A11,   # 󰨑
    0xF08B5,   # 󰢵
    0xF0B84,   # 󰮄
    0xF19A5,   # 󱦥
    0xEEF5,    # 
    0xF0820,   # 󰠠
    0xF15FC,   # 󱗼
    0xF15FF,   # 󱗿
    0xF15FE,   # 󱗾
    0xF1617,   # 󱘗
    0xF18A1,   # 󱢡
    0xF08D1,   # 󰣑
    0xF25B,    # 
    0xF0A50,   # 󰩐
    0xF0A51,   # 󰩑
    0x20       # Space
]


icons = "".join(chr(c) for c in codepoints)
print(f"Generating font from {font_path} with {len(codepoints)} icons...")

cmd1 = [
    "qmk", "painter-make-font-image",
    "-u", icons,
    "-n",
    "-s", "32",
    "-o", "fonts/nerd_32.png",
    "-f", font_path
]
subprocess.run(cmd1, check=True)

cmd2 = [
    "qmk", "painter-convert-font-image",
    "-f", "mono2",
    "-u", icons,
    "-n",
    "-i", "fonts/nerd_32.png",
    "-o", "fonts/"
]
subprocess.run(cmd2, check=True)
print("Success!")
