/*

tt_fillsinks.c

*/

#include "matrix.h"
#include "mex.h"
#include "topotoolbox.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
  // INPUTS: DEM, bc
  // OUTPUTS: DEMF
  if (nrhs != 2) {
    mexErrMsgIdAndTxt("tt3:tt_fillsinks:nrhs", "Two inputs required");
  }
  if (nlhs != 1) {
    mexErrMsgIdAndTxt("tt3:tt_fillsinks:nlhs", "One output required");
  }

  const mxArray *demArray = prhs[0];
  float *dem = mxGetSingles(demArray);

  const mxArray *bcArray = prhs[1];
  uint8_t *bc = mxGetUint8s(prhs[1]);

  ptrdiff_t dims[2];
  dims[0] = mxGetM(demArray);
  dims[1] = mxGetN(demArray);

  // Null pointer checks are not needed because out-of-memory errors
  // will terminate the MEX function.
  plhs[0] = mxCreateNumericMatrix(dims[0], dims[1], mxSINGLE_CLASS, mxREAL);
  float *out = mxGetSingles(plhs[0]);

  // Allocate the intermediate array for the FIFO queue used by the hybrid
  // algorithm
  mxArray *queueArray =
      mxCreateNumericMatrix(dims[0], dims[1], mxINT64_CLASS, mxREAL);
  ptrdiff_t *queue = (ptrdiff_t *)mxGetInt64s(queueArray);

  fillsinks_hybrid(out, queue, dem, bc, dims);

  mxDestroyArray(queueArray);
}
