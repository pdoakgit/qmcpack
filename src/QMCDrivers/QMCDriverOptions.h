#ifndef QMCPLUSPLUS_QMCDRIVEROPTIONS_H
#define QMCPLUSPLUS_QMCDRIVEROPTIONS_H

/** \file
 *
 *  Initially this will just replace the QMCDriverMode bitset
 *  with something sane
 */

#include <map>

namespace qmcplusplus
{
class QMCDriverOptions
{
  enum class Keys
  {
    UPDATE_MODE,    /**< walker or pbyp */
    MULTIPLE_MODE,  /**< bit for multple configuration */
    SPACEWARP_MODE, /**< bit for space-warping */
    ALTERNATE_MODE, /**< bit for performing various analysis and weird qmc methods */
    GPU_MODE,
    QMC_MULTIPLE,
    QMC_OPTIMIZE,
    QMC_WARMUP
  };

  std::map<Keys, bool>
}

} // namespace qmcplusplus
#endif
