#ifndef __TSMESSAGE_HANDLER_IMPL_H__
#define __TSMESSAGE_HANDLER_IMPL_H__

#include <iostream>
#include <string>

#include <ts_messages.pb.h>


//----------------------------------------------------------
// Added for GRPC (consider moving to core library)
void groupisSetRequestImpl (const thresholdsignature::GroupIsSetRequest& tsreq );
//----------------------------------------------------------


#endif //#ifndef __TSMESSAGE_HANDLER_IMPL_H__
