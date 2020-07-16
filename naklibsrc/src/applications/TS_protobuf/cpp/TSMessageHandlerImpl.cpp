
#include <TSMessageHandlerImpl.h>
#include <player.h>
#include <GroupMetaData.h>
#include <orchestrator.h>


// --------------------------------------------
// Handle Message helpers
// --------------------------------------------
void groupisSetRequestImpl (const thresholdsignature::GroupIsSetRequest& tsreq )
{
    
    // hard code for now
    BigNumber mod;
    mod.FromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");
    playerGroupMetaData playerGrp (tsreq.degree(), mod);
    
    playerGrp.m_id      = tsreq.groupid();

    for (int j = 0; j < tsreq.participants_size(); j++) {
        const thresholdsignature::GroupIsSetRequest::Participant& tsplayer = tsreq.participants(j);
        if(tsplayer.playerid().name() ==  SinglePlayer::Instance()->UserID()){
            playerGrp.m_ordinal = tsplayer.ordinal();
        }else
        {
            player p 
                (
                    tsplayer.playerid().name(),
                    tsplayer.playerid().uri(),
                    tsplayer.playerid().addr(),
                    tsplayer.playerid().port()
                ); 
            playerGrp.m_ordinalList.push_back( tsplayer.ordinal() ) ;            
            playerGrp.m_ordinalAndPlayerList.push_back(std::make_pair(tsplayer.ordinal(),p));
        }
    }

                
    std::cout << "TSMessageHandlerImpl..Handling broadcast group details message\n"
                << "\n\tGroup ID : " << playerGrp.m_id
                << "\n\tPlayer ordinal : " << playerGrp.m_ordinal
                << "\n\t player ordinal list: "
                ;
    for( auto iter : playerGrp.m_ordinalList ) 
    {
        std::cout << iter << " " ;
    }
    
    
    std::cout << std::endl; 
    playerGrp.polynomialPreCalculation(playerGrp.m_privateKeyPolynomial);
    
    SinglePlayer::Instance()->addPlayerGroup(playerGrp);

    return ;
}
