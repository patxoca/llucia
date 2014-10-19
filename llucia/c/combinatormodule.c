/* batch_combinations(iterable, r, n): retorna un generador que
 * generarà un bloc de N combinacions sense repetició agafant R
 * elements de ITERABLE. Cridades succesives retornen blocs de
 * combinacions consecutius.
 *
 * combinador(iterable, seed, n)
 */


#include "Python.h"

/* Objecte lot_combinacions *************************************************/

typedef struct {
    PyObject_HEAD
    Py_ssize_t *indices;        /* index dels elementes seleccionats */
    PyObject   *result;         /* most recently returned result tuple */
    Py_ssize_t n;               /* nombre d'elements */
    Py_ssize_t r;               /* mida de la mostra */
    int        stopped;         /* 1 quan s'exhaureixen les combinacions */
    Py_ssize_t batch_size;      /* mida del lot */
} lot_combinacions_object;

static PyTypeObject lot_combinacions_type;


static PyObject *
lot_combinacions_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    lot_combinacions_object *co;
    Py_ssize_t n;
    Py_ssize_t r;
    Py_ssize_t *indices = NULL;
    Py_ssize_t i;
    Py_ssize_t batch_size;
    static char *kwargs[] = {"n", "r", "batch_size", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "nnn:lot_combinacions", kwargs,
                                     &n, &r, &batch_size))
        return NULL;

    if (n <= 0) {
        PyErr_SetString(PyExc_ValueError, "n must be positive");
        goto error;
    }

    if (r <= 0) {
        PyErr_SetString(PyExc_ValueError, "r must be positive");
        goto error;
    }

    if (batch_size <= 0) {
        PyErr_SetString(PyExc_ValueError, "batch_size must be positive");
        goto error;
    }

    indices = PyMem_Malloc(r * sizeof(Py_ssize_t));
    if (indices == NULL) {
        PyErr_NoMemory();
        goto error;
    }

    for (i=0; i<r; i++) {
        indices[i] = i;
    }

    /* create lot_combinacionsobject structure */
    co = (lot_combinacions_object *)type->tp_alloc(type, 0);
    if (co == NULL)
        goto error;

    co->indices = indices;
    co->result = NULL;
    co->n = n;
    co->r = r;
    co->stopped = r > n ? 1 : 0;
    co->batch_size = batch_size;

    return (PyObject *)co;

error:
    if (indices != NULL)
        PyMem_Free(indices);
    return NULL;
}

static void
lot_combinacions_dealloc(lot_combinacions_object *co)
{
    PyObject_GC_UnTrack(co);
    Py_XDECREF(co->result);
    if (co->indices != NULL)
        PyMem_Free(co->indices);
    Py_TYPE(co)->tp_free(co);
}

static int
lot_combinacions_traverse(lot_combinacions_object *co, visitproc visit, void *arg)
{
    Py_VISIT(co->result);
    return 0;
}

static PyObject *
lot_combinacions_next(lot_combinacions_object *co)
{
    Py_ssize_t *indices = co->indices;
    PyObject *result = co->result;
    Py_ssize_t n = co->n;
    Py_ssize_t r = co->r;
    Py_ssize_t batch_size = co->batch_size;
    Py_ssize_t i, j, k;

    if (co->stopped)
        return NULL;

    if (result == NULL) {
        /* On the first pass, initialize result tuple using the indices */
        result = PyTuple_New(r);
        if (result == NULL)
            goto empty;
        co->result = result;
        for (i=0; i<r ; i++) {
            PyTuple_SET_ITEM(result, i, Py_BuildValue("i", indices[i]));
        }
    } else {
        result = PyTuple_New(r);
        if (result == NULL)
            goto empty;
        co->result = result;

        for (k = 0; k < batch_size; k++) {
            /* Scan indices right-to-left until finding one that is not
               at its maximum (i + n - r). */
            for (i=r-1 ; i >= 0 && indices[i] == i+n-r ; i--)
                ;

            /* If i is negative, then the indices are all at
               their maximum value and we're done. */
            if (i < 0)
                goto empty;

            /* Increment the current index which we know is not at its
               maximum.  Then move back to the right setting each index
               to its lowest possible value (one higher than the index
               to its left -- this maintains the sort order invariant). */
            indices[i]++;
            for (j=i+1 ; j<r ; j++)
                indices[j] = indices[j-1] + 1;
        }
        /* Update the result tuple for the new indices
           starting with i, the leftmost index that changed */

        for (i=0 ; i<r ; i++) {
            PyTuple_SET_ITEM(result, i, Py_BuildValue("i", indices[i]));
        }
    }

    Py_INCREF(result);
    return result;

empty:
    co->stopped = 1;
    return NULL;
}

PyDoc_STRVAR(lot_combinacions_doc,
"lot_combinacions(n, r, mida_lot) --> lot de combinacions\n\
\n\
\
Generador que retorna la llavor per generar un lot de MIDA_LOT\n\
combinacions a partir d'un conjut de N elements trian R elements sense\n\
repetició.\n\
");

static PyTypeObject lot_combinacions_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "combinator.lot_combinacions",      /* tp_name */
    sizeof(lot_combinacions_object),    /* tp_basicsize */
    0,                                  /* tp_itemsize */
    /* methods */
    (destructor)lot_combinacions_dealloc,     /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_compare */
    0,                                  /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    0,                                  /* tp_call */
    0,                                  /* tp_str */
    PyObject_GenericGetAttr,            /* tp_getattro */
    0,                                  /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC |
        Py_TPFLAGS_BASETYPE,            /* tp_flags */
    lot_combinacions_doc,               /* tp_doc */
    (traverseproc)lot_combinacions_traverse,  /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    PyObject_SelfIter,                  /* tp_iter */
    (iternextfunc)lot_combinacions_next,      /* tp_iternext */
    0,                                  /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    0,                                  /* tp_init */
    0,                                  /* tp_alloc */
    lot_combinacions_new,               /* tp_new */
    PyObject_GC_Del,                    /* tp_free */
};


/* Objecte combinador *******************************************************/

typedef struct {
    PyObject_HEAD
    PyObject   *pool;           /* tupla de valors */
    Py_ssize_t *indices;        /* index dels elementes seleccionats */
    PyObject   *result;         /* most recently returned result tuple */
    Py_ssize_t r;               /* mida de la mostra */
    int        stopped;         /* 1 quan s'exhaureixen les combinacions */
    Py_ssize_t batch_size;      /* mida del lot */
} combinador_object;

static PyTypeObject combinador_type;


static PyObject *
combinador_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    combinador_object *co;
    Py_ssize_t n;
    Py_ssize_t r;
    PyObject *pool = NULL;
    PyObject *iterable = NULL;
    Py_ssize_t *indices = NULL;
    Py_ssize_t i;
    Py_ssize_t batch_size;
    PyObject *seed = NULL;
    PyObject *iseed = NULL;
    static char *kwargs[] = {"iterable", "seed", "batch_size", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "OOn:combinador", kwargs,
                                     &iterable, &seed, &batch_size))
        return NULL;

    if ((pool = PySequence_Tuple(iterable)) == NULL)
        goto error;
    n = PyTuple_GET_SIZE(pool);
    if ((iseed = PySequence_Tuple(seed)) == NULL)
        goto error;
    r = PyTuple_GET_SIZE(iseed);
    if (r == 0) {
        PyErr_SetString(PyExc_ValueError, "seed must be non-empty");
        goto error;
    }

    indices = PyMem_Malloc(r * sizeof(Py_ssize_t));
    if (indices == NULL) {
        PyErr_NoMemory();
        goto error;
    }

    for (i=0; i<r; i++) {
		/* falta verificar que indices[i] està dins els límits i que
		 * indices[i] < indices[j] si i < j
		 */
        PyObject *obj = PyTuple_GET_ITEM(iseed, i);
        indices[i] = PyInt_AsLong(obj);
        Py_XDECREF(obj);
        if (PyErr_Occurred()) {
            goto error;
        }
    }

    Py_XDECREF(iseed);
    iseed = NULL;

    /* create combinadorobject structure */
    co = (combinador_object *)type->tp_alloc(type, 0);
    if (co == NULL)
        goto error;

    co->pool = pool;
    co->indices = indices;
    co->result = NULL;
    co->r = r;
    co->stopped = r > n ? 1 : 0;
    co->batch_size = batch_size;

    return (PyObject *)co;

error:
    if (indices != NULL)
        PyMem_Free(indices);
    Py_XDECREF(pool);
    Py_XDECREF(iseed);
    return NULL;
}

static void
combinador_dealloc(combinador_object *co)
{
    PyObject_GC_UnTrack(co);
    Py_XDECREF(co->pool);
    Py_XDECREF(co->result);
    if (co->indices != NULL)
        PyMem_Free(co->indices);
    Py_TYPE(co)->tp_free(co);
}

static int
combinador_traverse(combinador_object *co, visitproc visit, void *arg)
{
    Py_VISIT(co->pool);
    Py_VISIT(co->result);
    return 0;
}

static PyObject *
combinador_next(combinador_object *co)
{
    PyObject *elem;
    PyObject *oldelem;
    PyObject *pool = co->pool;
    Py_ssize_t *indices = co->indices;
    PyObject *result = co->result;
    Py_ssize_t n = PyTuple_GET_SIZE(pool);
    Py_ssize_t r = co->r;
    Py_ssize_t i, j, index;

    if (co->stopped)
        return NULL;

    if (result == NULL) {
        /* On the first pass, initialize result tuple using the indices */
        result = PyTuple_New(r);
        if (result == NULL)
            goto empty;
        co->result = result;
        for (i=0; i<r ; i++) {
            index = indices[i];
            elem = PyTuple_GET_ITEM(pool, index);
            Py_INCREF(elem);
            PyTuple_SET_ITEM(result, i, elem);
        }
    } else {
        /* Copy the previous result tuple or re-use it if available */
        if (Py_REFCNT(result) > 1) {
            PyObject *old_result = result;
            result = PyTuple_New(r);
            if (result == NULL)
                goto empty;
            co->result = result;
            for (i=0; i<r ; i++) {
                elem = PyTuple_GET_ITEM(old_result, i);
                Py_INCREF(elem);
                PyTuple_SET_ITEM(result, i, elem);
            }
            Py_DECREF(old_result);
        }
        /* Now, we've got the only copy so we can update it in-place
         * CPython's empty tuple is a singleton and cached in
         * PyTuple's freelist.
         */
        assert(r == 0 || Py_REFCNT(result) == 1);

        /* Scan indices right-to-left until finding one that is not
           at its maximum (i + n - r). */
        for (i=r-1 ; i >= 0 && indices[i] == i+n-r ; i--)
            ;

        /* If i is negative, then the indices are all at
           their maximum value and we're done. */
        if (i < 0)
            goto empty;

        /* Increment the current index which we know is not at its
           maximum.  Then move back to the right setting each index
           to its lowest possible value (one higher than the index
           to its left -- this maintains the sort order invariant). */
        indices[i]++;
        for (j=i+1 ; j<r ; j++)
            indices[j] = indices[j-1] + 1;

        /* Update the result tuple for the new indices
           starting with i, the leftmost index that changed */
        for ( ; i<r ; i++) {
            index = indices[i];
            elem = PyTuple_GET_ITEM(pool, index);
            Py_INCREF(elem);
            oldelem = PyTuple_GET_ITEM(result, i);
            PyTuple_SET_ITEM(result, i, elem);
            Py_DECREF(oldelem);
        }
    }

	co->batch_size--;
	if (!co->batch_size) {
		co->stopped = 1;
	}
    Py_INCREF(result);
    return result;

empty:
    co->stopped = 1;
    return NULL;
}

PyDoc_STRVAR(combinador_doc,
"combinador(iterable, seed, batch_size) --> lot de combinacions\n\
\n\
REVISAR\n\
Return successive r-length combinador of elements in the iterable.\n\n\
combinador(range(4), 3) --> (0,1,2), (0,1,3), (0,2,3), (1,2,3)");

static PyTypeObject combinador_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "combinator.combinador",            /* tp_name */
    sizeof(combinador_object),          /* tp_basicsize */
    0,                                  /* tp_itemsize */
    /* methods */
    (destructor)combinador_dealloc,     /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_compare */
    0,                                  /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    0,                                  /* tp_call */
    0,                                  /* tp_str */
    PyObject_GenericGetAttr,            /* tp_getattro */
    0,                                  /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC |
        Py_TPFLAGS_BASETYPE,            /* tp_flags */
    combinador_doc,                     /* tp_doc */
    (traverseproc)combinador_traverse,  /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    PyObject_SelfIter,                  /* tp_iter */
    (iternextfunc)combinador_next,      /* tp_iternext */
    0,                                  /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    0,                                  /* tp_init */
    0,                                  /* tp_alloc */
    combinador_new,                     /* tp_new */
    PyObject_GC_Del,                    /* tp_free */
};

/* Codi de nivell de mòdul **************************************************/

PyDoc_STRVAR(module_doc,
"Funcions per generar combinacions.\n\
");

PyMODINIT_FUNC
initcombinator(void)
{
    int i;
    PyObject *m;
    char *name;
    PyTypeObject *typelist[] = {
        &combinador_type,
        &lot_combinacions_type,
        NULL
    };

    m = Py_InitModule3("combinator", NULL, module_doc);
    if (m == NULL)
        return;

    for (i=0 ; typelist[i] != NULL ; i++) {
        if (PyType_Ready(typelist[i]) < 0)
            return;
        name = strchr(typelist[i]->tp_name, '.');
        assert (name != NULL);
        Py_INCREF(typelist[i]);
        PyModule_AddObject(m, name+1, (PyObject *)typelist[i]);
    }
}
