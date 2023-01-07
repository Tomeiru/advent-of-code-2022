import functools


def good_order(xs, ys):
    typeX = type(xs)
    typeY = type(ys)
    print(f"type={typeX},{typeY}")
    print(f"value={xs},{ys}")
    if typeX == typeY and typeX == int:
        if xs < ys:
            return 1
        if xs == ys:
            return 2
        return 3
    if typeX == list and typeY == int:
        return good_order(xs, [ys])
    if typeX == int and typeY == list:
        return good_order([xs], ys)
    for ii in range(0, min(len(xs), len(ys))):
        valuue = good_order(xs[ii], ys[ii])
        if valuue == 1:
            return 1
        if valuue == 3:
            return 3
    if len(xs) < len(ys):
        return 1
    if len(xs) == len(ys):
        return 2
    if len(xs) > len(ys):
        return 3
    return 3


def compare(xs, ys):
    if (good_order(xs, ys) == 1):
        return 1
    if (good_order(xs, ys) == 2):
        return 0
    return -1


def first():
    file_content = open("input", "r").read().splitlines()
    parsed_elems = []
    total = 0
    while file_content != []:
        parsed_elems.append((eval(file_content.pop()), eval(file_content.pop())))
        if (file_content != []):
            file_content.pop()

    parsed_elems.reverse()

    for i, elem in enumerate(parsed_elems):
        value = good_order(elem[1], elem[0])
        print(f"{i + 1} = {value}")
        if value == 1 or value == 2 :
            total += i + 1

    print(total)

def second():
    file_content = open("input", "r").read().splitlines()
    parsed_elems = []
    while file_content != []:
        parsed_elems.append(eval(file_content.pop()))
        parsed_elems.append(eval(file_content.pop()))
        if (file_content != []):
            file_content.pop()
    parsed_elems.reverse()
    parsed_elems.append([[2]])
    parsed_elems.append([[6]])
    parsed_elems.sort(key=functools.cmp_to_key(compare), reverse=True)
    print((parsed_elems.index([[2]]) + 1) * (parsed_elems.index([[6]]) + 1))

second()