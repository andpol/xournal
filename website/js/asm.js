/**
 * C patterns
 *
 * @author Daniel Holden
 * @author Craig Campbell
 * @version 1.0.7
 */
Rainbow.extend('asm', [
    {
        'name': 'comment',
        'pattern': /;.+/gm
    },
    {
        'name': 'keyword.operator',
        'pattern': /(push|pop|ori|cli|in|out)/g
    },
    {
        'name': 'support.type',
        'pattern': /0x\d+/g
    },
    {
        'name': 'variable.class',
        'pattern': /r\d+/g
    },
]);
