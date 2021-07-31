# crossplatformsocket
<h2>My cross platform implementation of Sockets in Windows and Linux</h2>

<h3>Using CrossSocket</h3>
<div>#include "CrossSocket.h"</div>
<div>The Namespace is "CSWL"</div>
<h3>Classes in namespace CSWL (CSWL::*)</h3>
<h4>CrossSocket - Manages the whole socket structure.</h4>
<h4>Endpoint - Represents IP and PORT</h4>
<h4>IpAddress - Represents IP (V4 or V6)</h4>
<h3>ENUMs</h3>
<h4>IpVersion, AddressFamily, SocketType, IpProtocol, ServerOrClient</h4>
<h3>Functions of Class "CSWL::CrossSocket"</h3>
<h4>Constructor with settings (some of the enums)</h4>
<h4>actionSuccess()  -  check if the last action had an error</h4>
<h4>lastError()  -  get the last error</h4>
<h4>CrossSocket acceptCS()  -  accept a client of socket (if server)</h4>
<h4>bool connectCS(...)  -  connect by domain or Ip (V4 or V6) to a server</h4>
<h4>int receiveCS(unsigned char* buffer, int bufferSize)  -  receive the next package of data</h4>
<h4>int sendCS(const unsigned char* buffer, int bufferSize)  -  send the next package of data</h4>



