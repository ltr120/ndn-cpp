/**
 * @author: Jeff Thompson
 * See COPYING for copyright and distribution information.
 */

#ifndef NDN_TRANSPORT_HPP
#define	NDN_TRANSPORT_HPP

#include <vector>

namespace ndn {

class Face;  
class Transport {
public:
  /**
   * 
   * @param face Not a shared_ptr because we assume that it will remain valid during the life of this Transport object.
   */
  virtual void connect(Face &face);
  
  virtual void send(const unsigned char *data, unsigned int dataLength);
  
  void send(const std::vector<unsigned char> &data)
  {
    send(&data[0], data.size());
  }
  
  /**
   * Make one pass to receive any data waiting on the connection.
   * @deprecated
   */
  virtual void tempReceive() = 0;

  /**
   * Close the connection.  This base class implementation does nothing, but your derived class can override.
   */
  virtual void close();
};

}

#endif
