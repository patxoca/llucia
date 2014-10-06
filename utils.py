# -*- coding: utf-8 -*-

# $Id$

import functools
import thread


def trace(logger):
    def decorator(f):
        name = f.__name__
        @functools.wraps(f)
        def wrapper(*args, **kw):
            thread_id = thread.get_ident()
            logger.debug(">>> %s[%i]", name, thread_id)
            res = f(*args, **kw)
            logger.debug("<<< %s[%i]", name, thread_id)
            return res
        return wrapper
    return decorator
