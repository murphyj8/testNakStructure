
#include <cstdlib>
#include <TSMessageFactory.h>
#include <orchestrator.h>
#include <GroupMetaData.h>
#include <player.h>
#include <TSProtoBufHelper.h>
#include <ts_messages.pb.h>



std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

//--------------------------------------------------------------
void createRegisterRequest(const player& p, thresholdsignature::RegisterRequest& registerreq)
{
    thresholdsignature::Player* tsplayer = new thresholdsignature::Player();
    tsplayer->set_name(p.userID());
    tsplayer->set_uri(p.uri()); 
    tsplayer->set_addr(p.addr()); 
    tsplayer->set_port(p.port()); 

    registerreq.set_allocated_playerid(tsplayer);

}

void createRegisterRequest(const player& p, std::ostream& os)
{
    thresholdsignature::RegisterRequest registerreq;
    createRegisterRequest( p, registerreq ) ;
    
    os << TSMessageDefs::TS_REGISTER_PLAYER_REQUEST;
    if (!registerreq.SerializeToOstream(&os)){
        throw std::runtime_error ("Could not serialize register message");
    }
    return ; 
}


//--------------------------------------------------------------
void createRegisterReply (const player& p, thresholdsignature::RegisterReply& resp)
{
    resp.set_success(true);
}

void createRegisterReply(const player& p, std::ostream& os)
{
    thresholdsignature::RegisterReply resp;
    createRegisterReply( p, resp ) ;

    os << TSMessageDefs::TS_REGISTER_PLAYER_RESPONSE; 
    if (!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize register player response");
    }  
    return ; 
}

//--------------------------------------------------------------
void createListPlayersRequest(const player& p, thresholdsignature::GetPlayerListRequest& playerlistreq)
{
    playerlistreq.set_name(p.userID());
}

void createListPlayersRequest(const player& p, std::ostream& os)
{
    thresholdsignature::GetPlayerListRequest playerlistreq;
    createListPlayersRequest( p, playerlistreq );

    os << TSMessageDefs::TS_LIST_PLAYER_REQUEST;
    if (!playerlistreq.SerializeToOstream(&os)){
        throw std::runtime_error ("Could not serialize player list message");
    }
    return ;  
}

//--------------------------------------------------------------
void createListPlayersResponse(const player& p, thresholdsignature::GetPlayerListResponse& resp)
{
    std::vector<player> players = getPlayerList(); 
    for(std::vector<player>::const_iterator iter = players.begin(); iter != players.end(); ++ iter){
        thresholdsignature::Player * tsp = resp.add_playerid();
        tsp->set_name(iter->userID());
        tsp->set_uri(iter->uri());
        tsp->set_addr(iter->addr());
        tsp->set_port(iter->port());
    }
}

void createListPlayersResponse(const player& p, std::ostream& os)
{
    thresholdsignature::GetPlayerListResponse resp ;
    createListPlayersResponse( p, resp ) ;

    os << TSMessageDefs::TS_LIST_PLAYER_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error( "Unable to serialize get user list response");
    }
    return ;
}

//--------------------------------------------------------------
void createGroupRequest(const player& p, const int& m, const int& n, thresholdsignature::CreateGroupRequest& grpreq)
{
    grpreq.set_userid(p.userID());
    grpreq.set_m(m);
    grpreq.set_n(n);
}

void createGroupRequest(const player& p, const int& m, const int& n, std::ostream& os)
{
    thresholdsignature::CreateGroupRequest grpreq;
    createGroupRequest(p, m, n, grpreq) ;

    os << TSMessageDefs::TS_CREATE_GROUP_REQUEST;
    if(!grpreq.SerializeToOstream(&os)){
        throw std::runtime_error ("Could not serialize create threshold group message" );
    }
    return ;
}

//--------------------------------------------------------------
void createGroupReply(const player& p, const std::string& grpid, const bool& grpset, thresholdsignature::CreateGroupReply& grpRsp)
{
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ;     
    id->set_userid( p.userID() );
    id->set_groupid( grpid );
    grpRsp.set_allocated_id( id ) ;
}

void createGroupReply(const player& p, const std::string& grpid, const bool& grpset, std::ostream& os)
{ 
    thresholdsignature::CreateGroupReply grpRsp;     
    createGroupReply( p, grpid, grpset, grpRsp ) ;

    os << TSMessageDefs::TS_CREATE_GROUP_RESPONSE;
    if(!grpRsp.SerializeToOstream(&os)){
        throw std::runtime_error( "Unabe to serialize a create threshold group response" );
    }
    return;
}


//--------------------------------------------------------------
void createInviteRequest( const std::string& grpid, thresholdsignature::InviteRequest& inviteReq)
{
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    id->set_groupid( grpid );
    inviteReq.set_allocated_id( id ) ;
}

void createInviteRequest( const std::string& grpid, std::ostream& os)
{
    thresholdsignature::InviteRequest inviteReq;
    createInviteRequest( grpid, inviteReq ) ;

    os << TSMessageDefs::TS_INVITE_TO_GROUP_REQUEST;
    if (!inviteReq.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize invite to threshold group message");
    }
    return ; 
}
 

//--------------------------------------------------------------
void createInviteReply 
    (const std::string& grpid, const std::string& playerid, const bool& accepted, thresholdsignature::InviteReply& inviteResp)
{
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    id->set_userid  ( playerid );
    id->set_groupid ( grpid );
    inviteResp.set_allocated_id( id ) ;    
    inviteResp.set_acceptance( accepted ); 
}

void createInviteReply
    (const std::string& grpid, const std::string& playerid, const bool& accepted, std::ostream& os)
{
    thresholdsignature::InviteReply inviteResp;
    createInviteReply( grpid, playerid, accepted, inviteResp ) ;

    os << TSMessageDefs::TS_INVITE_TO_GROUP_RESPONSE;
    if (!inviteResp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize invite to threshold group message");
    }
    return ; 
}

//--------------------------------------------------------------
void createGroupIsSetRequest
    (const std::string& userId, const GroupMetadata& grp, thresholdsignature::GroupIsSetRequest& tsreq)
{
    tsreq.set_groupid( grp.groupid() ) ;
    tsreq.set_degree( grp.t() ) ;

    int ordinal( 1 ) ;
    for ( auto iter : grp.participantList() ) 
    {        
        thresholdsignature::Player* id = new thresholdsignature::Player( ) ; 
        id->set_name( iter.m_userid ) ;
        id->set_uri ( iter.m_uri ) ;
        id->set_addr( iter.m_ip) ;
        id->set_port( iter.m_port ) ;
 
        thresholdsignature::GroupIsSetRequest::Participant* ptcpnt = tsreq.add_participants() ; 
        
        ptcpnt->set_ordinal  ( ordinal++ ) ;
        ptcpnt->set_allocated_playerid( id ) ;
    }
}

void createGroupIsSetRequest
    (const std::string& userId, const GroupMetadata& grp, std::ostream& os)
{

    std::cout << "In createGroupIsSetRequest" << std::endl ;

    thresholdsignature::GroupIsSetRequest tsreq;
    createGroupIsSetRequest( userId, grp, tsreq ) ;

    os << TSMessageDefs::TS_BROADCAST_GROUP_DETAILS_REQUEST ; 
    if(!tsreq.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize broadcast group details");
    }
    return;
}

//--------------------------------------------------------------
void createGroupIsSetReply
    (const std::string& grpid, const std::string& playerid, const bool& userset, thresholdsignature::GroupIsSetReply& resp)
{
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    id->set_userid( playerid );
    id->set_groupid( grpid );
    resp.set_allocated_id( id ) ;
    resp.set_success(userset);
}

void createGroupIsSetReply
    (const std::string& grpid, const std::string& playerid, const bool& userset, std::ostream& os)
{
    thresholdsignature::GroupIsSetReply resp;
    createGroupIsSetReply( grpid, playerid, userset, resp ) ;

    os << TSMessageDefs::TS_BROADCAST_GROUP_DETAILS_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("Unable to serialize broadcast group response");
    }
    return ; 
}

//--------------------------------------------------------------
void createDeleteTSGroupRequest(const player& p, const std::string& grpid, std::ostream& os){
    thresholdsignature::DeleteGroupRequest req; 
    req.set_userid(p.userID());
    req.set_groupid(grpid);
    os << TSMessageDefs::TS_DELETE_GROUP_REQUEST;
    if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize deleteGroup message for grp id " + grpid); 
    }
    return; 
}

void createDeleteTSGroupResponse(const player& p, const std::string& grpid, const bool& deleted, std::ostream& os){
    thresholdsignature::DeleteGroupResonse resp;
    resp.set_userid(p.userID());
    resp.set_groupid(grpid); 
    resp.set_groupdeleted(deleted);
    os << TSMessageDefs::TS_DELETE_GROUP_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize deleteGroupResponse message for grp id " + grpid); 
    }
    return ; 
}

void createDeleteTSPlayerGroupRequest(const std::string& userid, const std::string& grpid, std::ostream& os){
    thresholdsignature::DeleteGroupPlayerRequest req;
    req.set_userid(userid);
    req.set_groupid(grpid);
    os << TSMessageDefs::TS_DELETE_PLAYER_GROUP_REQUEST;
    if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize deleteTSPlayerGroupRequest message for grp id " + grpid);
    }
    return;
}

void createDeleteTSPlayerGroupResponse(const std::string& userid, const std::string& grpid, const bool& deleted, std::ostream& os){
    thresholdsignature::DeleteGroupPlayerResponse resp;
    resp.set_userid(userid);
    resp.set_groupid(grpid);
    resp.set_deleted(deleted);
    os << TSMessageDefs::TS_DELETE_PLAYER_GROUP_RESPONSE; 
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize deleteTSPlayerGroupResponse message for grp id " + grpid);
    }
    return ;
}







//--------------------------------------------------------------
void createSecretSharingRequest(const player& p, const std::string& grpid, const std::string& calctype,     thresholdsignature::ShareSecretRequest& req) 
{
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 

    id->set_userid          ( p.userID() );
    id->set_groupid         ( grpid );
    req.set_allocated_id    ( id ) ;
    req.set_calculation     ( string2enum( calctype ) ) ;

    return ; 
}

void createSecretSharingRequest(const player& p, const std::string& grpid, const std::string& calctype, std::ostream& os)
{
    thresholdsignature::ShareSecretRequest req;
    createSecretSharingRequest( p, grpid, calctype, req ) ;

    os << TSMessageDefs::TS_CREATE_GROUP_SECRET_REQUEST;
    if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialise SecretSharingRequest message for grp id grpid" + grpid); 
    }
    return ;  
}


//--------------------------------------------------------------
void createSecretSharingResponse(const player& p, const std::string& grpid, const std::string& calctype, const bool& sharedsecret, thresholdsignature::ShareSecretReply& resp)
{
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    
    id->set_userid          ( p.userID() );
    id->set_groupid         ( grpid );
    resp.set_allocated_id   ( id ) ;
    resp.set_calculation    ( string2enum( calctype ) ) ;
    resp.set_success        ( sharedsecret ) ;

    return;
}


void createSecretSharingResponse(const player& p, const std::string& grpid, const std::string& calctype, const bool& sharedsecret, std::ostream& os)
{
    thresholdsignature::ShareSecretReply resp;
    createSecretSharingResponse( p, grpid, calctype, sharedsecret, resp ) ;

    os << TSMessageDefs::TS_CREATE_GROUP_SECRET_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialise SecretSharingResponse message for group id" + grpid ); 
    }
    return;
}

//--------------------------------------------------------------
void createSecretSharingInitEvalRequest(const std::string& proposer, const std::string& grpid, const std::string& calctype, thresholdsignature::InitShareEvalsRequest& req)
{  
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 

    id->set_userid          ( proposer );
    id->set_groupid         ( grpid );
    req.set_allocated_id    ( id ) ;
    req.set_calculation     ( string2enum( calctype) );
    return;
}

void createSecretSharingInitEvalRequest(const std::string& proposer, const std::string& grpid, const std::string& calctype, std::ostream& os)
{    
    std::cout << "createSecretSharingInitEvalRequest" << std::endl ;
    thresholdsignature::InitShareEvalsRequest req;
    createSecretSharingInitEvalRequest( proposer, grpid, calctype, req) ;
    os << TSMessageDefs::TS_INITIATE_PRIVATE_EVAL_REQUEST;
    if(!req.SerializeToOstream(&os)){
        std::cout << "throwing from createSecretSharingInitEvalRequest" << std::endl ;
        throw std::runtime_error("could not serialize createSecretSharingInitEvalRequest for group id" + grpid);
    }
    return;
}

//--------------------------------------------------------------
void createSecretSharingInitEvalResponse(const std::string& userid, const std::string& grpid, const std::string& calctype, const bool& evalShareComplete, thresholdsignature::InitShareEvalsReply& resp)
{
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 

    id->set_userid          ( userid );
    id->set_groupid         ( grpid );
    resp.set_allocated_id   ( id ) ;
    resp.set_calculation    ( string2enum(calctype) ) ;
    resp.set_success        ( evalShareComplete );
    return;
}

void createSecretSharingInitEvalResponse(const std::string& userid, const std::string& grpid, const std::string& calctype, const bool& evalShareComplete, std::ostream& os){
    thresholdsignature::InitShareEvalsReply resp;
    createSecretSharingInitEvalResponse( userid, grpid, calctype, evalShareComplete, resp ) ;
    os << TSMessageDefs::TS_INITIATE_PRIVATE_EVAL_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        std::cout << "in createSecretSharingInitEvalResponse, throwing" << std::endl ;
        throw std::runtime_error("could not serialize SecretSharingInitEvalRequest for group id" + grpid);
    }
    return;
}

//--------------------------------------------------------------
void createPrivateDataEvalRequest(const std::string& userid, const std::string& grpid, const int& toOrdinal, const int& fromOrdinal, const jvrss& secrets, thresholdsignature::DistributeEvalsRequest& req)
{
    std::cout << "in createPrivateDataEvalRequest" << std::endl ;
    // look up the player ordinal in the vector of evals
    std::map<std::string, BigNumber>::const_iterator evalIter =  secrets.m_evals.find(std::to_string(toOrdinal));
    if(evalIter == secrets.m_evals.end()){
        std::cout << "In createPrivateDataEvalRequest, throwing" << std::endl ;
        throw std::runtime_error("No evalution for player ordinal " + std::to_string(toOrdinal) + " in the list of evaluated players");
    } 
    std::cout << "continuting" << std::endl ;

    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 

    id->set_userid          ( userid );
    id->set_groupid         ( grpid );
    req.set_allocated_id    ( id ) ;
    req.set_toordinal       ( toOrdinal );
    req.set_fromordinal     ( fromOrdinal );
    req.set_f_x(evalIter->second.ToHex());
    return;
}

void createPrivateDataEvalRequest(const std::string& userid, const std::string& grpid, const int& toOrdinal, const int& fromOrdinal, const jvrss& secrets, std::ostream& os)
{
    thresholdsignature::DistributeEvalsRequest req;
    createPrivateDataEvalRequest( userid, grpid, toOrdinal, fromOrdinal, secrets, req ) ;
    os << TSMessageDefs::TS_CREATE_PRIVATE_EVAL_REQUEST;
    if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize PrivateDataEvalRequest for player " + std::to_string(toOrdinal) );
    }
    return;
}

//--------------------------------------------------------------
void createRxAllEvalsRequest(const std::string& userid, const std::string& grpid, int& fromOrdinal, thresholdsignature::RxAllEvalsRequest& req )
{
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 

    id->set_userid          ( userid );
    id->set_groupid         ( grpid );
    req.set_allocated_id    ( id ) ;
    req.set_ordinal         ( fromOrdinal ) ;
    return ;
}

void createRxAllEvalsRequest(const std::string& userid, const std::string& grpid, int& fromOrdinal, std::ostream& os )
{
    thresholdsignature::RxAllEvalsRequest req ;
    createRxAllEvalsRequest( userid, grpid, fromOrdinal, req ) ;

    os << TSMessageDefs::TS_CREATE_RX_ALL_EVALS_REQUEST ;
    if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize RxAllEvalsReply for player " + userid );
    }
    return;
}

//--------------------------------------------------------------
void createRxAllEvalsResponse(const std::string& userid, const std::string& grpid, const bool& success, thresholdsignature::RxAllEvalsReply& resp )
{
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 

    id->set_userid          ( userid );
    id->set_groupid         ( grpid );
    resp.set_allocated_id   ( id ) ;
    resp.set_success        ( success );
    return;

}

void createRxAllEvalsResponse(const std::string& userid, const std::string& grpid, const bool& success, std::ostream& os )
{
    thresholdsignature::RxAllEvalsReply req ;
    createRxAllEvalsResponse( userid, grpid, success, req ) ;

    os << TSMessageDefs::TS_CREATE_RX_ALL_EVALS_RESPONSE ;
    if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize RxAllEvalsReply for player " + userid );
    }
    return;

}

//--------------------------------------------------------------
void createPrivateDataEvalResponse(const std::string& userid, const std::string& grpid, const bool& evaladded, thresholdsignature::DistributeEvalsReply& resp)
{
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 

    id->set_userid          ( userid );
    id->set_groupid         ( grpid );
    resp.set_allocated_id   ( id ) ;
    resp.set_success        ( evaladded );
    return;
}

void createPrivateDataEvalResponse(const std::string& userid, const std::string& grpid, const bool& evaladded, std::ostream& os)
{
    thresholdsignature::DistributeEvalsReply resp;
    createPrivateDataEvalResponse( userid, grpid, evaladded, resp ) ;

    os << TSMessageDefs::TS_INITIATE_PRIVATE_EVAL_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize PrivateDataEvalResponse for player " + userid );
    }
    return;
}

//--------------------------------------------------------------
void createSecretSharingPlayerDataRequest(const std::string& p, const std::string& grpid, const std::string& calctype, thresholdsignature::ShareSecretDataRequest& req)
{
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    id->set_userid          ( p );
    id->set_groupid         ( grpid );
    req.set_allocated_id    ( id ) ;
    req.set_calculation     ( string2enum( calctype ) );
    return;
}

void createSecretSharingPlayerDataRequest(const std::string& p, const std::string& grpid, const std::string& calctype, std::ostream& os){

    thresholdsignature::ShareSecretDataRequest req;
    createSecretSharingPlayerDataRequest( p, grpid, calctype, req ) ;
    os << TSMessageDefs::TS_CREATE_PLAYER_SECRET_REQUEST;
    if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialise SecretSharingPlayerDataRequest for player p " + p + "and group id: " + grpid);
    }
    return;
}

//--------------------------------------------------------------
void createSecretSharingPlayerDataResponse(const std::string& p, const std::string& grpid, const std::string& calctype , const int& ord, const jvrss& secrets, thresholdsignature::ShareSecretDataReply& resp)
{    
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    id->set_userid          ( p );
    id->set_groupid         ( grpid );
    resp.set_allocated_id   ( id ) ;

    resp.set_ordinal        ( ord ) ;
    resp.set_calculation    ( string2enum( calctype ) );

    for(
            std::vector<ECPoint>::const_iterator hiddenPolyIter =  secrets.m_hiddenPolynomial.begin();
            hiddenPolyIter != secrets.m_hiddenPolynomial.end(); 
            ++hiddenPolyIter
        )
    {
        resp.add_hiddenpoly( hiddenPolyIter->ToHex() ) ; 
    }

    std::map<std::string, std::vector<std::pair<std::string,ECPoint> > >::const_iterator 
        hiddenEvalIter = secrets.m_hiddenEvals.find(std::to_string(ord) ) ;
    if ( hiddenEvalIter != secrets.m_hiddenEvals.end() ) 
    {

        // Create Hidden Evals
        for(
                std::vector<std::pair<std::string, ECPoint> >::const_iterator ecptIter = hiddenEvalIter->second.begin(); 
                ecptIter != hiddenEvalIter->second.end(); 
                ++ ecptIter
            )
        {
            thresholdsignature::evaluatedPoly* ep = resp.add_hiddenevals() ;
            ep->set_ordinal ( std::stoi( ecptIter->first ) ) ;
            ep->set_f_x     ( ecptIter->second.ToHex() ) ;
        }
    }
    return;
}

void createSecretSharingPlayerDataResponse(const std::string& p, const std::string& grpid, const std::string& calctype , const int& ord, const jvrss& secrets, std::ostream& os)
{   
    thresholdsignature::ShareSecretDataReply resp;
    createSecretSharingPlayerDataResponse( p, grpid, calctype, ord, secrets, resp ) ;
    os << TSMessageDefs::TS_CREATE_PLAYER_SECRET_RESPONSE ; 
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialise SecretSharingPlayerDataResponse for player " + p + " and group " + grpid);
    }    
    return;
}


//--------------------------------------------------------------
void createSecretSharingCollatedDataRequest(const std::string& p, const std::string& grpid, const std::string& calctype, const GroupMetadata& grp, thresholdsignature::CollatedSecretRequest& req)
{
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    id->set_userid          ( p );
    id->set_groupid         ( grpid );
    req.set_allocated_id    ( id ) ;
    req.set_calculation( string2enum(calctype) ) ;
    

    // hiddenPolys
    for ( auto const& iter : grp.CollatedHiddenPolys() ) 
    {
        thresholdsignature::hiddenPolynomial* hp = req.add_hiddenpolys( ) ;
        hp->set_ordinal( std::stoi( iter.first ) ) ;

        for ( auto const& innerIter : iter.second ) 
        {
            hp->add_coefficients( innerIter.ToHex() ) ;
        }
    }

    // hiddenEvals
    for ( auto const& iter : grp.CollatedHiddenEvals() )
    {
        thresholdsignature::listOfPolynomials* lp = req.add_hiddenevals( ) ;        
        lp->set_ordinal( std::stoi( iter.first ) ) ;


        for ( auto const& innerIter : iter.second ) 
        {
            thresholdsignature::evaluatedPoly* ep = lp->add_ep( ) ;
            ep->set_ordinal( std::stoi( innerIter.first) ) ;
            ep->set_f_x( innerIter.second.ToHex() ) ;
        }
    }
    return ; 
}

void createSecretSharingCollatedDataRequest(const std::string& p, const std::string& grpid, const std::string& calctype, const GroupMetadata& grp, std::ostream& os)
{
    thresholdsignature::CollatedSecretRequest req;    
    createSecretSharingCollatedDataRequest( p, grpid, calctype, grp, req ) ; 
    os << TSMessageDefs::TS_CREATE_COLLATED_SECRET_PLAYER_REQUEST;
    if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize SecretSharingCollatedDataRequest for player " + p + " and group " + grpid);
    }
    return ; 
}

//--------------------------------------------------------------
void createSecretSharingCollatedPlayerResponse(const std::string& p, const std::string& grpid, const std::string& calctype, thresholdsignature::CollatedSecretReply& resp)
{
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    id->set_userid          ( p );
    id->set_groupid         ( grpid );
    resp.set_allocated_id   ( id ) ;
    resp.set_calculation( string2enum( calctype ) ) ;
    resp.set_success( true ) ;
    return ; 
}

void createSecretSharingCollatedPlayerResponse(const std::string& p, const std::string& grpid, const std::string& calctype,std::ostream& os)
{
    thresholdsignature::CollatedSecretReply resp; 
    createSecretSharingCollatedPlayerResponse( p, grpid, calctype, resp ) ;
    os << TSMessageDefs::TS_CREATE_COLLATED_SECRET_PLAYER_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize Collated Secret Reply for player " + p + " and group " + grpid);
    }    
    return ; 
}

//--------------------------------------------------------------
void createEphemeralKeyDataRequest(const std::string& p, const std::string& grpid, thresholdsignature::InitPresignRequest& req)
{
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    id->set_userid          ( p );
    id->set_groupid         ( grpid );
    req.set_allocated_id   ( id ) ;
    return;
}

void createEphemeralKeyDataRequest(const std::string& p, const std::string& grpid, std::ostream& os)
{
    thresholdsignature::InitPresignRequest req;
    createEphemeralKeyDataRequest( p, grpid, req ) ;
     os << TSMessageDefs::TS_CREATE_PRE_SIG_REQUEST;
    if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize InitPresignRequest for player " + p + " and group " + grpid);
    }
    return;
}

//--------------------------------------------------------------
void createEphemeralKeyDataResponse(const std::string& p, const std::string& grpid, thresholdsignature::InitPresignReply& resp)
{
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    id->set_userid          ( p );
    id->set_groupid         ( grpid );
    resp.set_allocated_id   ( id ) ;
    resp.set_success(true);
    return ;
}

void createEphemeralKeyDataResponse(const std::string& p, const std::string& grpid, std::ostream& os)
{
    thresholdsignature::InitPresignReply resp;
    createEphemeralKeyDataResponse( p, grpid, resp ) ;
    os << TSMessageDefs::TS_CREATE_PRE_SIG_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize InitPresignReply for player " + p + " and group " + grpid);
    }
    return ;
}


//--------------------------------------------------------------
void createEphermalKeyPlayerDataRequest(const std::string& p, const std::string& grpid, thresholdsignature::ShareVWDataMessageRequest& req)
{
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    id->set_userid          ( p );
    id->set_groupid         ( grpid );
    req.set_allocated_id   ( id ) ;
    return ; 
}

void createEphermalKeyPlayerDataRequest(const std::string& p, const std::string& grpid, std::ostream& os)
{
    thresholdsignature::ShareVWDataMessageRequest resp;
    createEphermalKeyPlayerDataRequest( p, grpid, resp ) ;
    os << TSMessageDefs::TS_CREATE_PRE_SIG_PLAYER_REQUEST;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize ShareVWDataMessageRequest for player " + p + " and group " + grpid);
    }
    return ; 
}


//--------------------------------------------------------------
void createEphemeralKeyPlayerDataResponse(const std::string& p, const std::string & grpid, const int& ordinal, const std::pair<BigNumber, ECPoint>& ekey, thresholdsignature::ShareVWDataMessage& resp)
{    
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    id->set_userid          ( p );
    id->set_groupid         ( grpid );
    resp.set_allocated_id   ( id ) ;  

    thresholdsignature::VWData* vw = new thresholdsignature::VWData( ) ;
    vw->set_ordinal( ordinal ) ;
    vw->set_v( ekey.first.ToHex() ) ;
    vw->set_w( ekey.second.ToHex() ) ;
    resp.set_allocated_data ( vw ) ;
    return ; 
}

void createEphemeralKeyPlayerDataResponse(const std::string& p, const std::string & grpid, const int& ordinal, const std::pair<BigNumber, ECPoint>& ekey, std::ostream& os)
{
    thresholdsignature::ShareVWDataMessage resp;
    createEphemeralKeyPlayerDataResponse( p, grpid, ordinal, ekey, resp ) ;
    os << TSMessageDefs::TS_CREATE_PRE_SIG_PLAYER_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize EphemeralKeyPlayerDataResponse for player " + p + " and group " + grpid);
    }
    return ; 
}


//--------------------------------------------------------------
void createEphemeralKeyCollatedDataRequest(const std::string& p, const std::string& grpid, const GroupMetadata& grp, thresholdsignature::CollatedVWShareRequest& req)
{
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    id->set_userid          ( p );
    id->set_groupid         ( grpid );
    req.set_allocated_id    ( id ) ;

    for(
            std::map<std::string, std::pair<BigNumber, ECPoint> >::const_iterator iter = grp.CollatedVW().begin(); 
            iter != grp.CollatedVW().end(); 
            ++ iter
        )
    {
        thresholdsignature::VWData* ptrOrdVW = req.add_data();
        ptrOrdVW->set_ordinal(std::stoi( iter->first));
        ptrOrdVW->set_v(iter->second.first.ToHex());
        ptrOrdVW->set_w(iter->second.second.ToHex());
    }
    return ; 
}

void createEphemeralKeyCollatedDataRequest(const std::string& p, const std::string& grpid, const GroupMetadata& grp, std::ostream& os)
{
    thresholdsignature::CollatedVWShareRequest req;
    createEphemeralKeyCollatedDataRequest( p, grpid, grp, req ) ;
    os << TSMessageDefs::TS_CREATE_COLLATED_PRE_SIG_REQUEST;
    if(!req.SerializeToOstream(&os)){
       throw std::runtime_error("could not serialize AllEphemeralKeyPlayerRequest for player " + p + " and group " + grpid);
    }
    return ; 
}

//--------------------------------------------------------------
void createAEphemeralKeyCollatedDataResponse(const std::string& p, const std::string& grpid, const std::string& ordinal, thresholdsignature::CollatedVWShareReply& resp)
{   
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    id->set_userid          ( p );
    id->set_groupid         ( grpid );
    resp.set_allocated_id   ( id ) ;    
    resp.set_ordinal( std::stoi( ordinal ) ) ;

    // need a way to track & report failures
    resp.set_success(true);
    return ;
}

void createAEphemeralKeyCollatedDataResponse(const std::string& p, const std::string& grpid, const std::string& ordinal, std::ostream& os)
{
    thresholdsignature::CollatedVWShareReply resp;
    createAEphemeralKeyCollatedDataResponse( p, grpid, ordinal, resp ) ;
    os << TSMessageDefs::TS_CREATE_COLLATED_PRE_SIG_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize AllEphemeralKeyPlayerResponse for player " + p + " and group " + grpid); 
    }
    return ;
}

//--------------------------------------------------------------
void createSignatureRequest(const std::string& p, const std::string& grpid, const std::string& msg, const int& eKeyIndex, thresholdsignature::InitSignatureRequest& sigreq)
{
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    id->set_userid          ( p );
    id->set_groupid         ( grpid );
    sigreq.set_allocated_id ( id ) ;  
    sigreq.set_message(msg);
    sigreq.set_keyindex(eKeyIndex);
    return ;
}

void createSignatureRequest(const std::string& p, const std::string& grpid, const std::string& msg, const int& eKeyIndex, std::ostream& os)
{
    thresholdsignature::InitSignatureRequest sigreq;
    createSignatureRequest( p, grpid, msg, eKeyIndex, sigreq ) ;
    os << TSMessageDefs::TS_CREATE_GROUP_SIG_REQUEST;
    if(!sigreq.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize InitSignatureRequest for player " + p + " and group " + grpid);
    }
    return ;
}

//--------------------------------------------------------------
void createSignatureResponse(const std::string& p, const std::string& grpid, const GroupMetadata& grp , const std::string& msg, const int& ekeyindex, thresholdsignature::SignDataMessage& sigresp)
{
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    id->set_userid              ( p );
    id->set_groupid             ( grpid );
    sigresp.set_allocated_id    ( id ) ;  
    sigresp.set_message(msg);
    sigresp.set_keyindex(ekeyindex);
    for(std::vector<std::pair<std::string, BigNumber> >::const_iterator iter = grp.CollatedPartialSignatures().begin(); iter != grp.CollatedPartialSignatures().end(); ++iter){
        thresholdsignature::SigData* siginfo = sigresp.add_signatures();
        siginfo->set_ordinal(std::stoi( iter->first) );
        siginfo->set_signature(iter->second.ToHex());
    }
    return;
}

void createSignatureResponse(const std::string& p, const std::string& grpid, const GroupMetadata& grp , const std::string& msg, const int& ekeyindex,std::ostream& os)
{
    thresholdsignature::SignDataMessage sigresp;
    createSignatureResponse( p, grpid, grp, msg, ekeyindex, sigresp ) ;
    os << TSMessageDefs::TS_CREATE_GROUP_SIG_RESPONSE;
    if(!sigresp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialise GenerateSignatureResponse for player " + p + " and group " + grpid);
    }
    return;
}


//--------------------------------------------------------------
void createSignaturePlayerDataRequest(const std::string& p, const std::string& grpid, const std::string& themessage, const int& ekeyIndex, thresholdsignature::ShareOfSigRequest& playersigreq)
{
    playersigreq.set_groupid ( grpid ) ;  
    playersigreq.set_message(themessage);
    playersigreq.set_keyindex(ekeyIndex);
    return ; 
}

void createSignaturePlayerDataRequest(const std::string& p, const std::string& grpid, const std::string& themessage, const int& ekeyIndex, std::ostream& os)
{
    thresholdsignature::ShareOfSigRequest playersigreq;
    createSignaturePlayerDataRequest( p, grpid, themessage, ekeyIndex, playersigreq ) ;
    os << TSMessageDefs::TS_CREATE_SIG_PLAYER_REQUEST;
    if(!playersigreq.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize SignaturePlayerDataRequest for player " + p + " and group " + grpid);
    }
    return ; 
}

//--------------------------------------------------------------
void createSignaturePlayerDataResponse(const std::string& p, const std::string& grpid, const std::pair<std::string, BigNumber>& siginfo, const int& ekeyindex, thresholdsignature::ShareOfSigReply& playersigresp)
{
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    id->set_userid          ( p ) ;
    id->set_groupid         ( grpid ) ;
    playersigresp.set_allocated_id ( id ) ;  
    playersigresp.set_ordinal(std::stoi( siginfo.first ) );
    playersigresp.set_keyindex(ekeyindex);
    playersigresp.set_signature(siginfo.second.ToHex());
    return ;
}

void createSignaturePlayerDataResponse(const std::string& p, const std::string& grpid, const std::pair<std::string, BigNumber>& siginfo, const int& ekeyindex,std::ostream& os){
    thresholdsignature::ShareOfSigReply playersigresp;
    createSignaturePlayerDataResponse( p, grpid, siginfo, ekeyindex, playersigresp ) ;
    os << TSMessageDefs::TS_CREATE_SIG_PLAYER_RESPONSE;
    if(!playersigresp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize SignaturePlayerDataResponse for player " + p + " and group " + grpid);
    }
    return ;
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void createPrivateKeyRequest(const std::string& p,  const std::string& grpid, const std::vector<std::string>& users, std::ostream& os){
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::GenerateGroupPrivateKeyRequest priKeyReq;
    id->set_userid          ( p ) ;
    id->set_groupid         ( grpid ) ;
    priKeyReq.set_allocated_id ( id ) ;  
    
    for(std::vector<std::string>::const_iterator iter = users.begin(); iter != users.end(); ++ iter){
        priKeyReq.add_players( *iter ) ;
    }
    os << TSMessageDefs::TS_CREATE_PRIKEY_REQUEST;
    if(!priKeyReq.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize GenerateGroupPrivateKeyRequest for player " + p + " and group " + grpid);
    }
    return;
}

void createPrivateKeyResponse(const std::string& p, const std::string& grpid, const bool& keyshared, std::ostream& os){
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::GenerateGroupPrivateKeyResponse prikeyresp;
    id->set_userid          ( p ) ;
    id->set_groupid         ( grpid ) ;
    prikeyresp.set_allocated_id ( id ) ;  
    
    prikeyresp.set_privatedatashared(keyshared);
    os << TSMessageDefs::TS_CREATE_PRIKEY_RESPONSE;
    if(!prikeyresp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize GenerateGroupPrivateKeyResponse for player " + p + " and group " + grpid);
    }
    return;
}
 
 void createPlayerPrivateShareExchangeRequest(const std::string& p, const std::string& grpid, std::ostream& os){
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::PlayerPrivateShareExchangeRequest req;
    id->set_userid          ( p ) ;
    id->set_groupid         ( grpid ) ;
    req.set_allocated_id ( id ) ;  
    
    os << TSMessageDefs::TS_CREATE_PRIKEYSHARE_EXCHANGE_REQUEST;
    if(!req.SerializeToOstream(&os)){
       throw std::runtime_error("could not serialize PlayerPrivateShareExchangeRequest for player " + p + " and group " + grpid);
    }
    return;
 }

 void createPlayerPrivateShareExchangeResponse(const std::string& p, const std::string& grpid, const bool& keyshared, std::ostream& os){
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::PlayerPrivateShareExchangeResponse resp;
    id->set_userid          ( p ) ;
    id->set_groupid         ( grpid ) ;
    resp.set_allocated_id ( id ) ;  

     resp.set_privatedatashared(keyshared);
     os << TSMessageDefs::TS_CREATE_PRIKEYSHARE_EXCHANGE_RESPONSE ;
     if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize PlayerPrivateShareExchangeResponse for player " + p + " and group " + grpid);
    }
    return;

 }

 void createPlayerPrivateKeyShareRequest(const playerGroupMetaData& grpinfo,std::ostream& os){

    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::PlayerPrivateKeyShareRequest req;
    id->set_userid          ( getPublicPlayerInfo().userID());
    id->set_groupid         ( grpinfo.m_id );
    req.set_allocated_id ( id ) ;  


    req.set_ordinal(grpinfo.m_ordinal);
    req.set_privatekeyshare(grpinfo.privateKeyShare().ToHex());
    os << TSMessageDefs::TS_CREATE_PLAYER_PRIKEY_REQUEST;
    if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize PlayerPrivateKeyShareRequest for player " + getPublicPlayerInfo().userID() + " and group " + grpinfo.m_id);
    }
    return;
 }

void createPlayerPrivateKeyShareResponse(const std::string& userid, const std::string& grpid, const bool& sharerec, std::ostream& os){
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::PlayerPrivateKeyShareResponse resp;
    id->set_userid          ( userid );
    id->set_groupid         ( grpid );
    resp.set_allocated_id ( id ) ;  

    resp.set_sharedprivatekeyshare(sharerec);
    os << TSMessageDefs::TS_CREATE_PLAYER_PRIKEY_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize PlayerPrivateKeyShareResponse for player " + userid + " and group " + grpid);
    }
    return;
}

