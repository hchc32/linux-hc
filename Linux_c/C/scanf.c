int __cdecl scanf (
        const char *format,
        ...
        )
{
        va_list arglist;
        va_start(arglist, format);
        return vscanf_fn(_input_l, format, NULL, arglist);
}

int __cdecl vscanf_fn (
        INPUTFN inputfn,
        const char *format,
        _locale_t plocinfo,
        va_list arglist
        )
/*
 * stdin 'SCAN', 'F'ormatted
 */
{
    int retval = 0;

    _VALIDATE_RETURN( (format != NULL), EINVAL, EOF);

    _lock_str2(0, stdin);
    __try {
        retval = (inputfn(stdin, format, plocinfo, arglist));
    }
    __finally {
        _unlock_str2(0, stdin);
    }

    return(retval);
}
