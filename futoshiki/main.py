import math
import re
import os

class Cell:
    def __init__(self):
        self.less_up = False
        self.greater_up = False
        self.less_left = False
        self.greater_left = False
        self.less_down = False
        self.greater_down = False
        self.less_right = False
        self.greater_right = False
        self.value = 0
        self.possible = set()

    def __init__(self, value=0, left=0, right=0, down=0, up=0):
        self.less_up = False
        self.greater_up = False
        self.less_left = False
        self.greater_left = False
        self.less_down = False
        self.greater_down = False
        self.less_right = False
        self.greater_right = False
        self.value = 0
        self.possible = set()

        if up == 1:
            self.greater_up = True
        elif up == -1:
            self.less_up = True

        if down == 1:
            self.greater_down = True
        elif down == -1:
            self.less_down = True

        if left == 1:
            self.greater_left = True
        elif left == -1:
            self.less_left = True

        if right == 1:
            self.greater_right = True
        elif right == -1:
            self.less_right = True

    def __str__(self):
        result = 'cell: '
        if self.less_up:
            result += 'less_up '
        if self.greater_up:
            result += 'greater_up '
        if self.less_left:
            result += 'less_left '
        if self.greater_left:
            result += 'greater_left '
        if self.less_down:
            result += 'less_down '
        if self.greater_down:
            result += 'greater_down '
        if self.less_right:
            result += 'less_right '
        if self.greater_right:
            result += 'greater_right '
        return result



class Field:

    def __init__(self):
        self.N = 0
        self.cells = []

    def __init__(self, filename):
        re_less = re.compile('(\d+) (\d+) < (\d+) (\d+)')
        re_greater = re.compile('(\d+) (\d+) > (\d+) (\d+)')
        re_number = re.compile('(\d+) (\d+) = (\d+)')
        # try:
        input_file = open(filename, 'r')
        self.N = int(input_file.readline())
        self.cells = [[Cell() for _ in range(self.N)] for __ in range(self.N)]

        for line in input_file.readlines():
            line = line.replace('\n', '')
            check_less = re_less.match(line)
            check_greater = re_greater.match(line)
            check_number = re_number.match(line)
            if check_less != None:
                x1, y1, x2, y2 = map(lambda s: int(s) - 1, [check_less.group(index) for index in range(1, 5)])
                if x1 < x2 and y1 == y2:
                    self.cells[x1][y1].less_down = True
                    self.cells[x2][y2].greater_up = True
                elif x1 > x2 and y1 == y2:
                    self.cells[x1][y1].less_up = True
                    self.cells[x2][y2].greater_down = True
                elif x1 == x2 and y1 < y2:
                    self.cells[x1][y1].less_right = True
                    self.cells[x2][y2].greater_left = True
                elif x1 == x2 and y1 > y2:
                    self.cells[x1][y1].less_left = True
                    self.cells[x2][y2].greater_right = True
                else:
                    pass
            elif check_greater != None:
                x1, y1, x2, y2 = map(lambda s: int(s) - 1, [check_greater.group(index) for index in range(1, 5)])
                if x1 < x2 and y1 == y2:
                    self.cells[x1][y1].greater_down = True
                    self.cells[x2][y2].less_up = True
                elif x1 > x2 and y1 == y2:
                    self.cells[x1][y1].greater_up = True
                    self.cells[x2][y2].less_down = True
                elif x1 == x2 and y1 < y2:
                    self.cells[x1][y1].greater_right = True
                    self.cells[x2][y2].less_left = True
                elif x1 == x2 and y1 > y2:
                    self.cells[x1][y1].greater_left = True
                    self.cells[x2][y2].less_right = True
                else:
                    pass
            elif check_number != None:
                x, y, n = map(lambda s: int(s) - 1, [check_number.group(index) for index in range(1, 4)])
                self.cells[x][y].value = n + 1
            else:
                pass
        # except:
        #     print('can\' open or parse file')

    def __str__(self):
        result = ''
        '''
        for i in range(self.N):
            for j in range(self.N):
                if self.cells[i][j].less_down:
                    result += '{0} {1} < {2} {3}\n'.format(i, j, i + 1, j)
                if self.cells[i][j].greater_down:
                    result += '{0} {1} > {2} {3}\n'.format(i, j, i + 1, j)
                if self.cells[i][j].less_right:
                    result += '{0} {1} < {2} {3}\n'.format(i, j, i, j + 1)
                if self.cells[i][j].greater_right:
                    result += '{0} {1} > {2} {3}\n'.format(i, j, i, j + 1)
        result += '\n'
        for i in range(self.N):
            for j in range(self.N):
                if self.cells[i][j].value > 0:
                    result += '{0} '.format(self.cells[i][j].value)
                else:
                    result += '- '
            result += '\n'
        '''
        if self.N == 0:
            return result
        #------------------------------------
        for j in range(0, self.N - 1):

            for i in self.cells[j][:-1]:
                splitter = '|'
                if i.less_right:
                    splitter = '<'
                elif i.greater_right:
                    splitter = '>'
                ch = '*'
                if i.value > 0:
                    ch = str(i.value)
                result += '{0} {1} '.format(ch, splitter)
            ch = str(self.cells[j][self.N - 1].value)
            if ch == '0':
                result += '*' + '\n'
            else:
                result += ch + '\n'

            for i in range(self.N - 1):
                ch = '-'
                if self.cells[j ][i].greater_down:
                    ch = 'v'
                elif self.cells[j][i].less_down:
                    ch = '^'
                result += ch + ' - '
            ch = '-'
            if self.cells[j ][self.N - 1].greater_down:
                ch = 'v'
            elif self.cells[j ][self.N - 1].less_down:
                ch = 'ʌ'
            result += ch + '\n'

        for i in self.cells[-1][:-1]:
            splitter = '|'
            if i.less_right:
                splitter = '<'
            elif i.greater_right:
                splitter = '<'
            ch = '*'
            if i.value > 0:
                ch = str(i.value)
            result += '{0} {1} '.format(ch, splitter)
        ch = str(self.cells[0][self.N - 1].value)
        if ch == '0':
            result += '*' + '\n'
        else:
            result += ch + '\n'
        #------------------------------------

        return result

    def calc_less(self):
        self.less_control = [[[-1, self.N] for _ in range(self.N)] for __ in range(self.N)]
        for i in range(self.N):
            for j in range(self.N):
                self._less_DFS(i, j)


    def calc_greater(self):
        self.greater_control = [[[-1, 1] for _ in range(self.N)] for __ in range(self.N)]
        for i in range(self.N):
            for j in range(self.N):
                self._greater_DFS(i, j)


    def _less_DFS(self, x, y):
        if not ((0 <= x < self.N) and (0 <= y < self.N)):
            return
        if self.less_control[x][y][0] != -1:
            return

        result = 0
        max_num = self.N
        if 1 <= self.cells[x][y].value <= self.N:
            max_num = self.cells[x][y].value

        if self.cells[x][y].less_up:
            self._less_DFS(x - 1, y)
            result = max(result, 1 + self.less_control[x - 1][y][0])
            max_num = min(max_num, -1 + self.less_control[x - 1][y][1])
        if self.cells[x][y].less_down:
            self._less_DFS(x + 1, y)
            result = max(result, 1 + self.less_control[x + 1][y][0])
            max_num = min(max_num, -1 + self.less_control[x + 1][y][1])
        if self.cells[x][y].less_right:
            self._less_DFS(x, y + 1)
            result = max(result, 1 + self.less_control[x][y + 1][0])
            max_num = min(max_num, -1 + self.less_control[x][y + 1][1])
        if self.cells[x][y].less_left:
            self._less_DFS(x, y - 1)
            result = max(result, 1 + self.less_control[x][y - 1][0])
            max_num = min(max_num, -1 + self.less_control[x][y - 1][1])

        self.less_control[x][y] = [result, max_num]


    def _greater_DFS(self, x, y):
        if not ((0 <= x < self.N) and (0 <= y < self.N)):
            return
        if self.greater_control[x][y][0] != -1:
            return

        result = 0
        min_num = 1
        if 1 <= self.cells[x][y].value <= self.N:
            min_num = self.cells[x][y].value

        if self.cells[x][y].greater_up:
            self._greater_DFS(x - 1, y)
            result = max(result, 1 + self.greater_control[x - 1][y][0])
            min_num = max(min_num, 1 + self.greater_control[x - 1][y][1])
        if self.cells[x][y].greater_down:
            self._greater_DFS(x + 1, y)
            result = max(result, 1 + self.greater_control[x + 1][y][0])
            min_num = max(min_num, 1 + self.greater_control[x + 1][y][1])
        if self.cells[x][y].greater_right:
            self._greater_DFS(x, y + 1)
            result = max(result, 1 + self.greater_control[x][y + 1][0])
            min_num = max(min_num, 1 + self.greater_control[x][y + 1][1])
        if self.cells[x][y].greater_left:
            self._greater_DFS(x, y - 1)
            result = max(result, 1 + self.greater_control[x][y - 1][0])
            min_num = max(min_num, 1 + self.greater_control[x][y - 1][1])

        self.greater_control[x][y] = [result, min_num]

    def calc_possibilities(self):
        flag = False
        min_var = self.N
        min_i = 0
        min_j = 0
        self.calc_greater()
        self.calc_less()
        for i in range(self.N):
            for j in range(self.N):
                self.cells[i][j].possible = set()
                start = self.greater_control[i][j][1]
                stop = self.less_control[i][j][1]
                if start > stop:
                    return None
                for k in range(start, stop + 1):
                    self.cells[i][j].possible.add(k)
                for t in range(self.N):
                    if i != t and 1 <= self.cells[t][j].value <= self.N:
                        self.cells[i][j].possible.discard(self.cells[t][j].value)
                for t in range(self.N):
                    if j != t and 1 <= self.cells[i][t].value <= self.N:
                        self.cells[i][j].possible.discard(self.cells[i][t].value)
                if len(self.cells[i][j].possible) < min_var and self.cells[i][j].value == 0:
                    flag = True
                    min_var = len(self.cells[i][j].possible)
                    min_i = i
                    min_j = j
        '''
        for i in range(self.N):
            for j in range(self.N):
                print(i, j, self.cells[i][j].possible)
            print()
        '''

        if flag:
            return [min_var, min_i, min_j]
        else:
            return [0, 0, 0]

    def is_full(self):
        for i in self.cells:
            for j in i:
                if not (1 <= j.value <= self.N):
                    return False
        return True

    def solve(self):
        if self.is_full():
            print(self)
            return True
        p = self.calc_possibilities()
        if p[0] == 0:
            return False
        p_set = self.cells[p[1]][p[2]].possible.copy()
        '''
        print(self)
        print(p, p_set)
        input()
        '''
        flag = False
        for i in p_set:
            self.cells[p[1]][p[2]].value = i
            if self.solve():
                flag = True
            self.cells[p[1]][p[2]].value = 0
        return flag


F = Field('example.txt')
print(F.solve())
