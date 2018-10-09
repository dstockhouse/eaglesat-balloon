/*****************************************************************************
 *
 * File:
 * 	crp_python.c
 *
 * Description:
 * 	Functions related to interfacing to the CRP, if function to talk to 
 * 	the sensor are in a python module in the directory
 *
 * Revision 1.0
 * 	Date: 10/9/18
 *	Author: David Stockhouse
 *	Changes: Created from template
 *
 ****************************************************************************/

#include "crp.h"
#include "generateFilename.h"

#include <Python.h>

#include <stdio.h>
#include <stdlib.h>


/**** Function crp_sensorRead ****
 * Reads an image from the sensor and places it into the buffer.
 */
int crp_sensorRead(char *buffer, int bufSize) {

	PyObject *pythonModuleName, *pythonModule;

	// Initialize use of the embedded python module
	Py_Initialize();

	// Load python module
	pythonModuleName = PyString_FromString("crp");
	if(pythonModuleName == NULL) {
		printf("Could not fetch python module name \"crp\".\n");
		return 1;
	}

	pythonModule = PyImport_Import(pythonModuleName);
	if(pythonModule == NULL) {
		printf("Could not load python module \"crp\".\n");
		return 2;
	}

	// Close out use of python
	Py_Finalize();

	return 0;

} // Function crp_sensorRead


/**** Function crp_imageStore ****
 * Stores an image that has been read from a buffer into the file described
 * by df.
 */
int crp_imageStore(char *buffer, int bufSize, FILE *fd) {


	return 0;

} // Function crp_imageStore

