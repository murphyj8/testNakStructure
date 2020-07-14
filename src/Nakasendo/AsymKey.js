var binary = require('node-pre-gyp');//
var path = require('path')
var binding_path = binary.find(path.resolve(path.join(__dirname,'../../package.json')));
var nakesendoBindings = require(binding_path)

var{
    ECPoint,
    AddECPoint,
    EC_Compare,
    EC_Double,
    EC_Invert,
    EC_ScalerMultiply,
    EC_MultiplyScalerByGenerator
} = require('./ECPoint.js')


class AsymKey{
    #m_PriKey;
    #m_PubKey;
    constructor(){
        var keyArr = nakesendoBindings.GenerateKeyPairPEM();
        this.#m_PriKey=keyArr[1];
        this.#m_PubKey = keyArr[0];
    }
    
    
    
    get pubkey(){
        return this.#m_PubKey; 
    }
    
    FromEncryptedPEMStr (keyPemForm, passPhrase){
        var keyArr = nakesendoBindings.ImportFromEncryptedPEM(keyPemForm, passPhrase);
        this.#m_PubKey = keyArr[0];
        this.#m_PriKey = keyArr[1];
     }

    ToEncryptedPEMStr (passPhrase){
        return nakesendoBindings.ExportFromEncryptedPEM(passPhrase);
    }
            

    derivePrivateKey(msg){
        return nakesendoBindings.DerivePrivate(this.#m_PriKey, msg);
    }
        
     
    derivePublicKey(msg){
        return nakesendoBindings.DerivePublic(this.#m_PubKey, msg);
    }
        
    SplitKey(threshold=10,shares=100){
        return nakesendoBindings.SplitKey(this.#m_PriKey, threshold, shares);
    }
               
    RecoverKey(shares){
        var keys = nakesendoBindings.RestoreKey(shares);
        this.#m_PubKey = keys[0];
        this.#m_PriKey = keys[1];
    }
        
        
    sign(msg){
        return nakesendoBindings.Sign ( msg, this.#m_PriKey)
    }
        
    CalculateSharedSecret(pubkey){
        return nakesendoBindings.ShareSecret(this.#m_PriKey, pubkey)
    }
    
    toString(){
        return this.#m_PubKey; 
    }
    
    PubKeyHex (){
        var key = nakesendoBindings.ExportKeyPairHEX(this.#m_PriKey);
        return key[0];
    }
    
}

function verify(msg, pubkey, rval, sval){
    return nakesendoBindings.Verify(msg, pubkey, rval,sval);
}

function verifyPubKeyHex(msg, pubkey, rval, sval){
    pubKeyPt = new ECPoint();
    pubKeyPt.SetValue =  pubkey;
    var coords = pubKeyPt.GetAffineCoordinates();
    pubkeypem = pubKeyHexPtstoPEM(coords['x'],coords['y']);
    return nakesendoBindings.Verify(msg, pubkeypem, rval,sval);
}

function verifyDER(msg, pubkey, sigDER){
    return nakesendoBindings.VerifyDER(msg,pubkey,sigDER);
}
function verifyDERPubKeyHex(msg, pubkey, sigDER){
    pubKeyPt = new ECPoint();
    pubKeyPt.SetValue =  pubkey;
    var coords = pubKeyPt.GetAffineCoordinates();
    pubkeypem = pubKeyHexPtstoPEM(coords['x'],coords['y']);
    return nakesendoBindings.VerifyDER(msg,pubkeypem,sigDER);
}
    
function createDERFormat(rValue, sValue,isdecimal){
    // check that the decimal flag is set both ways?
    hexSig = nakesendoBindings.DERSignature(rValue, sValue,isdecimal);
    return hexSig;
}

function pubKeyAsHexPt(pubkey, iscompressed=false){
    pt = nakesendoBindings.PubKeyPEMToHex(pubkey, iscompressed);
    return pt;
}

function pubKeyHexPtstoPEM(xVal, yVal,nid=714){
    pemStr = nakesendoBindings.pubKeyHextoPEM(xVal,yVal,nid);
    return pemStr;
}

module.exports = {
    AsymKey,
    verify,
    verifyPubKeyHex,
    createDERFormat,
    verifyDER,
    pubKeyAsHexPt,
    pubKeyHexPtstoPEM
}

