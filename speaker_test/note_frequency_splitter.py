NOTES = ('A', 'A#', 'B', 'C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#')

def freq(note):
    if len(note) == 3:
        octave = int(note[2])
    else:
        octave = int(note[1])
    key_no = NOTES.index(note[:-1])

    if key_no < 3:
        key_no = key_no + 12 + ((octave - 1) * 12) + 1
    else:
        key_no = key_no + ((octave - 1) * 12) + 1

    return 440 * (2 ** ((key_no - 49) / 12))

ms = "D – F – D – F – D – F – D – F – D – F – D – F – D – F – D – F – D# – G – D# – G – C# – G – D – F – D – E – C# – E – D"
print("{", end = "")
for i in ms.split("–"):
    i = i.lstrip().rstrip() 
    i += "5"
    # print(i, end = ", ")
    # print()
    print(int(freq(i)), end = ", ")
print("}")