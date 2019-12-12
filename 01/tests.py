import subprocess

def check(expr, result):
    out = subprocess.run(['./calc', expr], stdout = subprocess.PIPE)
    if out.stdout.decode('ascii') != result:
        print('Error {0} != {1}, got {2}'.format(expr, result, out.stdout))
    else:
        print('OK')

#spaces and unary minus
print('Spaces and unary minus:')
check('2', '2\n')
check('    3', '3\n')
check('-2', '-2\n')
check('   -3', '-3\n')
check('   -4    ', '-4\n')
check('--5', '5\n')
check('---5', '-5\n')
check('  -    --   5353', '-5353\n')

#binary operations
print('Binary operations:')
check('4*7', '28\n')
check('12/4', '3\n')
check('20+30', '50\n')
check('20-30', '-10\n')

#priority
print('Priority:')
check('2+3*4', '14\n')
check('100/5/2', '10\n')
check('100/5*2', '40\n')
check('2*100/5', '40\n')
check('-2*5', '-10\n')
check('2*-5', '-10\n')
check('-40/8', '-5\n')
check('40/-8', '-5\n')
check('-40/-8', '5\n')
check('2 + 3 * 4 - -2', '16\n')

#invalid
print('Invalid input:')
check('', 'Incorrect using of operation\n')
check('-', 'Incorrect using of operation\n')
check('+', 'Incorrect using of operation\n')
check('*', 'Incorrect using of operation\n')
check('/', 'Incorrect using of operation\n')
check('+2', 'Incorrect using of operation\n')
check('++2', 'Incorrect using of operation\n')
check('+-2', 'Incorrect using of operation\n')
check('-+2', 'Incorrect using of operation\n')
check('-2*', 'Incorrect using of operation\n')
check('2 3', 'Incorrect symbol found\n')
check('alpha', 'Incorrect symbol found\n')
check('1a', 'Incorrect symbol found\n')
check('a1', 'Incorrect symbol found\n')
check('1    a', 'Incorrect symbol found\n')
check('   1   + 3  * 5ab', 'Incorrect symbol found\n')
check('30 * / 2', 'Incorrect using of operation\n')
check('30 / * 3', 'Incorrect using of operation\n')
check('200-/-50', 'Incorrect using of operation\n')
check(' 200    -  ', 'Incorrect using of operation\n')
check(' 1 / 0', 'Division by zero!\n')
check('  100 / 2 * 3 / 0 - 7', 'Division by zero!\n')
#mix
print('Mix:')
check('2323  +    294 -   - 2949  * 21200 /-200 * -25', '7817467\n')
check(' -  -    --  - - 768    / 3   -- 16 + 25 * 29999', '750247\n')
check('-012 + 17 - 0000024', '-19\n')
