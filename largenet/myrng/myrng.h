/**
 * MyRNG main include file
 *
 * @file myrng.h
 */
#ifndef MYRNG_H_
#define MYRNG_H_

#include "RandomVariates.h"
#include "WELLEngine.h"

/**
 * @mainpage MyRNG: A convenient random number generator
 *
 * @section about About
 * The MyRNG C++ classes provide two very efficient random number generators
 * and several methods for generating random variates. You can choose between
 * the Mersenne Twister MT19937a algorithm in the implementation of Makoto Matsumoto
 * and Takuji Nishimura and the well-equidistributed long-period linear generator WELL1024a
 * implemented by Marton Morvai. Both generators are essentially shuffled linear
 * congruential random number generators; they should not be used for cryptography
 * applications but should otherwise cover most scientific needs. While the WELL1024a is
 * slightly faster in the current implementation, the MT19937a has an astronomical recurrence
 * time of 2^19937. Furthermore, the methods are provided to generate several random variates
 * including uniform, Gaussian, Beta, and Gamma distributions. Generation of most random
 * variates is based on Law and Kelton, 2000. Generation of the Gamma distribution is based
 * on Marsagli and Tsang, 2000.
 *
 * @section getting-started Getting started
 * To use the class include <em>either</em> myrngMT.h (MT19937a) <em>or</em> myrngWELL.h (WELL1024a)
 * in your code. Including the header files creates an instance of the appropriate
 * myrng::RandomVariates class called @p rng. Then, random variates can be obtained by calling
 * the corresponding member functions.
 *
 * For instance, a random variable from a uniform distribution between 0 and 1 can be obtained as
 * @code
 * double u = rng.Uniform01();
 * @endcode
 * and a random variable form an exponential distribution with mean @c m as
 * @code
 * double m = 0.7;
 * double v = rng.Exponential(m);
 * @endcode
 *
 * If you prefer to create your own instance of the random number generator instead of using a
 * global variable, do <em>not</em> include the above header files. For this purpose, use
 * @code
 * #include <myrng.h>
 * myrng::WELL1024a rng_well; // random variates using the WELL1024a generator
 * myrng::MT19937 rng_mt;     // random variates using the MT19937 generator
 * double x = rng_well.Exponential(0.3);
 * double y = rng_mt.Exponential(0.7);
 * @endcode
 *
 * For further information see the full source code documentation.
 *
 * @par Disclaimer:
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * The myrng namespace.
 */
namespace myrng
{
	/**
	 * Random variates using the WELL1024a engine.
	 */
	typedef RandomVariates<WELLEngine> WELL1024a;

}

#endif /* MYRNG_H_ */
