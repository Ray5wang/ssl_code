/*******************************************************************************
 * ManyEars: Matrix - Header                                                   *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: Fran�ois Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Bri�re                                                 *
 * Version: 1.1.0                                                              *
 * Date: August 31st, 2010                                                     *
 *                                                                             *
 * Disclaimer: This software is provided "as is". Use it at your own risk.     *
 *                                                                             *
 *******************************************************************************
 *                                                                             *
 * ManyEars has been created and developped at:                                *
 *                                                                             *
 * IntroLab, Universite de Sherbrooke, Sherbrooke, Quebec, Canada              *
 *                                                                             *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * The following articles relate to the ManyEars project:                      *
 *                                                                             *
 * S. Bri�re, J.-M. Valin, F. Michaud, Dominic L�tourneau, Embedded Auditory   *
 *     System for Small Mobile Robots, Proc. International Conference on       *
 *     Robotics and Automation (ICRA), 2008.                                   *
 *                                                                             *
 * J.-M. Valin, S. Yamamoto, J. Rouat, F. Michaud, K. Nakadai, H. G. Okuno,    *
 *     Robust Recognition of Simultaneous Speech By a Mobile Robot, IEEE       *
 *     Transactions on Robotics, Vol. 23, No. 4, pp. 742-752, 2007.            *
 *                                                                             *
 * J.-M. Valin, F. Michaud, J. Rouat, Robust Localization and Tracking of      *
 *     Simultaneous Moving Sound Sources Using Beamforming and Particle        *
 *     Filtering. Robotics and Autonomous Systems Journal (Elsevier), Vol. 55, *
 *     No. 3, pp. 216-228, 2007.                                               *
 *                                                                             *
 * S. Yamamoto, K. Nakadai, M. Nakano, H. Tsujino, J.-M. Valin, K. Komatani,   *
 *     T. Ogata, H. G. Okuno, Simultaneous Speech Recognition based on         *
 *     Automatic Missing-Feature Mask Generation integrated with Sound Source  *
 *     Separation (in Japanese). Journal of Robotic Society of Japan, Vol. 25, *
 *     No. 1, 2007.                                                            *
 *                                                                             *
 * J.-M. Valin, F. Michaud, J. Rouat, Robust 3D Localization and Tracking of   *
 *     Sound Sources Using Beamforming and Particle Filtering. Proc. IEEE      *
 *     International Conference on Acoustics, Speech and Signal Processing     *
 *     (ICASSP), pp. 841-844, 2006.                                            *
 *                                                                             *
 * S. Briere, D. Letourneau, M. Frechette, J.-M. Valin, F. Michaud, Embedded   *
 *     and integration audition for a mobile robot. Proceedings AAAI Fall      *
 *     Symposium Workshop Aurally Informed Performance: Integrating Machine    *
 *     Listening and Auditory Presentation in Robotic Systems, FS-06-01, 6-10, *
 *     2006                                                                    *
 *                                                                             *
 * S. Yamamoto, K. Nakadai, J.-M. Valin, J. Rouat, F. Michaud, K. Komatani,    *
 *     T. Ogata, H. G. Okuno, Making a robot recognize three simultaneous      *
 *     sentences in real-time. Proceedings of IEEE/RSJ International           *
 *     Conference on Intelligent Robots and Systems (IROS), 2005.              *
 *                                                                             *
 * M. Murase, S. Yamamoto, J.-M. Valin, K. Nakadai, K. Yamada, K. Komatani,    *
 *     T. Ogata, H. G. Okuno, Multiple Moving Speaker Tracking by Microphone   *
 *     Array on Mobile Robot. Proc. European Conference on Speech              *
 *     Communication and Technology (Interspeech), 2005.                       *
 *                                                                             *
 * S. Yamamoto, J.-M. Valin, K. Nakadai, J. Rouat, F. Michaud, T. Ogata, H.    *
 *     G. Okuno, Enhanced Robot Speech Recognition Based on Microphone Array   *
 *     Source Separation and Missing Feature Theory. Proc. International       *
 *     Conference on Robotics and Automation (ICRA), 2005.                     *
 *                                                                             *
 * J.-M. Valin, J. Rouat, F. Michaud, Enhanced Robot Audition Based on         *
 *     Microphone Array Source Separation with Post-Filter. Proc. IEEE/RSJ     *
 *     International Conference on Intelligent Robots and Systems (IROS),      *
 *     pp. 2123-2128, 2004.                                                    *
 *                                                                             *
 * J.-M. Valin, F. Michaud, J. Rouat, D. L�tourneau, Robust Sound Source       *
 *     Localization Using a Microphone Array on a Mobile Robot. Proc. IEEE/RSJ *
 *     International Conference on Intelligent Robots and Systems (IROS),      *
 *     pp. 1228-1233, 2003.                                                    *
 *                                                                             *
 * J.-M. Valin, F. Michaud, B. Hadjou, J. Rouat, Localization of Simultaneous  *
 *     Moving Sound Sources for Mobile Robot Using a Frequency-Domain Steered  *
 *     Beamformer Approach. Proc. IEEE International Conference on Robotics    *
 *     and Automation (ICRA), pp. 1033-1038, 2004.                             *
 *                                                                             *
 * J.-M. Valin, J. Rouat, F. Michaud, Microphone Array Post-Filter for         *
 *     Separation of Simultaneous Non-Stationary Sources. Proc. IEEE           *
 *     International Conference on Acoustics, Speech and Signal Processing     *
 *     (ICASSP), pp. 221-224, 2004.                                            *
 *                                                                             *
 ******************************************************************************/

#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include "../hardware.h"
#include "../Utilities/dynamicMemory.h"


/*******************************************************************************
 * Structure                                                                   *
 ******************************************************************************/

struct objMatrix
{

    // +-------------------------------------------------------------------+
    // | Variables                                                         |
    // +-------------------------------------------------------------------+

    // Number of rows in the matrix
    unsigned int nRows;

    // Number of columns in the matrix
    unsigned int nCols;

    // Number of frames (must be a multiple of 4)
    unsigned int nFrames;

    // Content of the matrix
    float*** valueReal;
    float*** valueImag;

};

/*******************************************************************************
 * Prototypes                                                                  *
 ******************************************************************************/

struct objMatrix* matrixCreate(unsigned int nRows, unsigned int nCols, unsigned int nFrames);

void matrixDelete(struct objMatrix* matrix);

void matrixInit(struct objMatrix* matrix, unsigned int nRows, unsigned int nCols, unsigned int nFrames);

void matrixTerminate(struct objMatrix* matrix);

float matrixGetReal(struct objMatrix* matrix, unsigned int indexRow, unsigned int indexCol, unsigned int k);

float matrixGetImag(struct objMatrix* matrix, unsigned int indexRow, unsigned int indexCol, unsigned int k);

void matrixSetReal(struct objMatrix* matrix, unsigned int indexRow, unsigned int indexCol, unsigned int k, float value);

void matrixSetImag(struct objMatrix* matrix, unsigned int indexRow, unsigned int indexCol, unsigned int k, float value);

void matrixInsertRow(struct objMatrix* matrix, unsigned int newRowIndex);

void matrixInsertCol(struct objMatrix* matrix, unsigned int newColIndex);

void matrixDeleteRow(struct objMatrix* matrix, unsigned int deleteRowIndex);

void matrixDeleteCol(struct objMatrix* matrix, unsigned int deleteColIndex);

void matrixMultMatrix(struct objMatrix* matrixA, struct objMatrix* matrixB, struct objMatrix* matrixResult);

void matrixMultScalar(struct objMatrix* matrixSource, float scalar, struct objMatrix* matrixResult);

void matrixRemoveIdentity(struct objMatrix* matrix);

void matrixRemoveDiagonal(struct objMatrix* matrix);

void matrixHermitian(struct objMatrix* matrixSource, struct objMatrix* matrixDest);

void matrixResize(struct objMatrix* matrix, unsigned int nRows, unsigned int nCols, unsigned int nFrames);

void matrixCopy(struct objMatrix* matrixSource, struct objMatrix* matrixDest);

void matrixMultMatrixPerElement(struct objMatrix* matrixSource, struct objMatrix* matrixB, struct objMatrix* matrixResult);

void matrixMultScalarPerFrame(struct objMatrix* matrixSource, struct objMatrix* matrixFrames, struct objMatrix* matrixResult);

void matrixAddMatrix(struct objMatrix* matrixA, struct objMatrix* matrixB, struct objMatrix* matrixResult);

void matrixDividePerElement(struct objMatrix* matrixSource, struct objMatrix* matrixB, struct objMatrix* matrixResult);

void matrixInvRealPerElement(struct objMatrix* matrixSource, struct objMatrix* matrixResult);

void matrixMakeNonZero(struct objMatrix* matrix);

void matrixPrint(struct objMatrix* matrix);

void matrixPrintOneFrame(struct objMatrix* matrix, unsigned int k);

#endif
