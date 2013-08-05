/**
 * C patterns
 *
 * @author Daniel Holden
 * @author Craig Campbell
 * @version 1.0.7
 */
Rainbow.extend('avr-asm', [
    {
        'name': 'keyword.operator',
        'pattern': /(XCALL|XJMP)/g
    },
    {
        'name': 'comment',
        'pattern': /\/\*+/g
    },
    {
        'name': 'support.type',
        'pattern': /(.section|.init9)/g
    },
    {
        'name': 'variable.class',
        'pattern': /OS_Init/g
    },
]);
