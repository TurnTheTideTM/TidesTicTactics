import cPickle

winP1 = 5
winP2 = -5
chance_bonus = 2
more_bonus = 0.1
mid_bonus = 0.0
minmaxscore = 3.5

winsMasks = [
    int('000000111', 2),
    int('000111000', 2),
    int('111000000', 2),
    int('100100100', 2),
    int('010010010', 2),
    int('001001001', 2),
    int('100010001', 2),
    int('001010100', 2)
]

chancesMasks = {
    int('000000011', 2): 2,
    int('000000101', 2): 1,
    int('000000110', 2): 0,

    int('000011000', 2): 5,
    int('000101000', 2): 4,
    int('000110000', 2): 3,

    int('011000000', 2): 8,
    int('101000000', 2): 7,
    int('110000000', 2): 6,

    int('000100100', 2): 8,
    int('100000100', 2): 5,
    int('100100000', 2): 2,

    int('000010010', 2): 7,
    int('010000010', 2): 4,
    int('010010000', 2): 1,

    int('000001001', 2): 6,
    int('001000001', 2): 3,
    int('001001000', 2): 0,

    int('000010001', 2): 8,
    int('100000001', 2): 4,
    int('100010000', 2): 0,

    int('000010100', 2): 6,
    int('001000100', 2): 4,
    int('001010000', 2): 2
}

midMask = int('000010000', 2)


def bewerteBoard(setP1, setP2):
    pop1 = popCount(setP1)
    pop2 = popCount(setP2)

    winP1Bool = False
    winP2Bool = False

    for mask in winsMasks:
        if (setP1 & mask) == mask:
            winP1Bool = True

    for mask in winsMasks:
        if (setP2 & mask) == mask:
            winP2Bool = True

    if winP1Bool and winP2Bool:
        return 0

    if winP1Bool:
        return winP1

    if winP2Bool:
        return winP2

    chancesP1 = 0
    for mask in chancesMasks:
        if (setP1 & mask) == mask:  # Wenn P1 eine potentielle chance hat
            for winMask in winsMasks:
                if popCount(winMask & mask) == 2:  # suche die winMask die auf die mask passt
                    if (setP2 & winMask) != 0:  # und ein teil von P2 darauf liegt
                        break;  # dann ist die position doch keine chance
            else:
                chancesP1 += 1

    chancesP2 = 0
    for mask in chancesMasks:
        if (setP2 & mask) == mask:  # Wenn P2 eine potentielle chance hat
            for winMask in winsMasks:
                if popCount(winMask & mask) == 2:  # suche die winMask die auf die mask passt
                    if (setP1 & winMask) != 0:  # und ein teil von P1 darauf liegt
                        break;  # dann ist die position doch keine chance
            else:
                chancesP2 += 1

    p1mid = setP1 & midMask
    p2mid = setP2 & midMask

    # BEWERTUNG -----------------

    ret = (pop1 - pop2) * more_bonus

    ret += chancesP1 * chance_bonus
    ret -= chancesP2 * chance_bonus

    if p1mid:
        ret += mid_bonus
    elif p2mid:
        ret -= mid_bonus

    return min(max(ret, -minmaxscore), minmaxscore)


def popCount(integer):
    return bin(integer).count('1')


file = open("MovegenTable.h", 'w')
file.write("#pragma once\nfloat movegenTable [] = {0\n")

summe = 0
arr = [0]

# COORDINATE:
#   8 bit
# max 9 coordinates
#   72 bit + 4bit length = 76 bit
# max 9 boards
#   576 bit

for setPlayerOne in range(2 ** 9):
    for setPlayerTwo in range(2 ** 9):
        if not setPlayerOne and not setPlayerTwo:
            continue
        score = bewerteBoard(setPlayerOne, setPlayerTwo)
        summe += score
        file.write("," + str(score) + "\n")
        arr.append(score)

print("Summe aller Scores: " + str(score))

file.write("};\n")
file.close()
# cPickle.dump(arr, open("RatingCPickle.table", 'w'))
