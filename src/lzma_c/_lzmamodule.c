#define Py_LIMITED_API 0x03110000 // Python 3.11
#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "LzmaLib.h"
#include "LzmaEnc.h"
#include "LzmaDec.h"
#include "Alloc.h"
#include "7zTypes.h"

static PyObject* lzma_compress_py(PyObject* self, PyObject* args) {
    Py_buffer src_buf;
    int level = 5, numThreads = 1;
    unsigned dictSize = 1 << 23;
    int lc = 3, lp = 0, pb = 2, fb = 128;

    if (!PyArg_ParseTuple(args, "y*|Iiiiii", &src_buf, &level, &dictSize, &lc, &lp, &pb, &fb, &numThreads)) {
        return NULL; 
    }
    size_t propsSize = LZMA_PROPS_SIZE;
    unsigned char props[LZMA_PROPS_SIZE];
    size_t destLen = src_buf.len + src_buf.len / 3 + 128;
    unsigned char* dest = (unsigned char*)PyMem_Malloc(destLen);
    if (!dest) return PyErr_NoMemory();

    int res = LzmaCompress(dest, &destLen, (const unsigned char*)src_buf.buf, src_buf.len,
                        props, &propsSize, level, dictSize, lc, lp, pb, fb, numThreads);

    PyBuffer_Release(&src_buf); 

    if (res != SZ_OK) {
        PyMem_Free(dest);
        PyErr_SetString(PyExc_RuntimeError, "LZMA compression failed");
        return NULL;
    }

    PyObject* result = Py_BuildValue("(y#y#)", dest, destLen, props, propsSize);
    PyMem_Free(dest);
    return result;
}
static PyObject* lzma_uncompress_py(PyObject* self, PyObject* args) {
    Py_buffer src_buf, props_buf;
    size_t uncompressed_size;

    if (!PyArg_ParseTuple(args, "y*y*K", &src_buf, &props_buf, &uncompressed_size)) {
        return NULL;
    }

    if (props_buf.len != LZMA_PROPS_SIZE) {
        PyErr_SetString(PyExc_ValueError, "LZMA properties must be 5 bytes");
        PyBuffer_Release(&src_buf);
        PyBuffer_Release(&props_buf);
        return NULL;
    }

    size_t destLen = uncompressed_size;
    unsigned char* dest = (unsigned char*)PyMem_Malloc(destLen);
    if (!dest) return PyErr_NoMemory();

    size_t srcLen = src_buf.len;
    int res = LzmaUncompress(dest, &destLen, (const unsigned char*)src_buf.buf, &srcLen,
                             (const unsigned char*)props_buf.buf, props_buf.len);

    PyBuffer_Release(&src_buf);
    PyBuffer_Release(&props_buf);

    if (res != SZ_OK) {
        PyMem_Free(dest);
        PyErr_SetString(PyExc_RuntimeError, "LZMA uncompression failed");
        return NULL;
    }

    PyObject* result = Py_BuildValue("y#", dest, destLen);
    PyMem_Free(dest);
    return result;
}


static PyMethodDef LzmaMethods[] = {
    {"compress", lzma_compress_py, METH_VARARGS, "Compress data using LZMA."},
    {"uncompress", lzma_uncompress_py, METH_VARARGS, "Decompress data using LZMA."},
    {NULL, NULL, 0, NULL} // Sentinel
};

static struct PyModuleDef lzmamodule = {
    PyModuleDef_HEAD_INIT,
    "_lzma", 
    "A Python interface to the 7-Zip LZMA C library.", 
    -1,
    LzmaMethods
};

PyMODINIT_FUNC PyInit__lzma(void) {
    return PyModule_Create(&lzmamodule);
}