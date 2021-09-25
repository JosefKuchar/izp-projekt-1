#!/usr/bin/python3
import os

test_cases = [
    # Original test cases
    ('cat hesla.txt', '1234567890\nPassword\nHeslo123\nMojevelmidlouhehesloscislem0\nIZP2021:fit@vut.cz\n'),
    ('./pwcheck 1 1 <hesla.txt', 'Password\nHeslo123\nMojevelmidlouhehesloscislem0\nIZP2021:fit@vut.cz\n'),
    ('./pwcheck 2 3 <hesla.txt', 'Heslo123\nMojevelmidlouhehesloscislem0\nIZP2021:fit@vut.cz\n'),
    ('./pwcheck 3 2 <hesla.txt', 'Heslo123\nMojevelmidlouhehesloscislem0\nIZP2021:fit@vut.cz\n'),
    ('./pwcheck 4 2 <hesla.txt', 'Heslo123\nIZP2021:fit@vut.cz\n'),
    ('./pwcheck 2 4 --stats <hesla.txt', 'IZP2021:fit@vut.cz\nStatistika:\nRuznych znaku: 36\nMinimalni delka: 8\nPrumerna delka: 14.4\n'),
    # Additional test cases
    ('echo | ./pwcheck 1 1 --stats', 'Statistika:\nRuznych znaku: 0\nMinimalni delka: 0\nPrumerna delka: 0.0\n')
]

for test_case in test_cases:
    output = os.popen(test_case[0]).read()

    if output == test_case[1]:
        print('[ OK ] {}'.format(test_case[0]))
    else:
        print('[FAIL] {}'.format(test_case[0]))
        print('Expected:')
        print(test_case[1])
        print('Got:')
        print(output)
