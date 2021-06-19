//
// Bo Li
// Caltech, Pasadena, CA, 91125
//

#if !defined(_M4EXTREME_FEMAPFILECONVERTER_H_)
#define _M4EXTREME_FEMAPFILECONVERTER_H_

#if defined(__cplusplus)

namespace m4extreme {

    extern "C" {
#endif /* __cplusplus */

        void
        FemapFileConverter(unsigned int DIM,
			   const char * inputFile,
			   const char * outputFile,
			   int type = 0);

#if defined(__cplusplus)
    }
}
#endif /* __cplusplus */

#endif
