#include "SecureHelpers.hpp"

namespace cg {

CryptoPP::SecByteBlock SecureHelpers::CopySecByteBlock(
	const CryptoPP::SecByteBlock & b)
{
	CryptoPP::SecByteBlock c(b.size());
	std::memcpy(c, b, b.size());
	return c;
}
void SecureHelpers::HashIt512(HashData & hashData)
{
	CryptoPP::SHA512 hash;
	if (hashData.m_salt.size() == 0)
		hashData.m_salt.Assign(GetSaltData(ms_defaultSaltSize));
	hash.Update(hashData.m_salt, hashData.m_salt.size());
	hash.Update(hashData.m_data, hashData.m_data.size());
	hashData.m_data.CleanNew(hash.DIGESTSIZE);
	hash.Final(hashData.m_data);
}
void SecureHelpers::HashIt256(HashData & hashData)
{
	CryptoPP::SHA256 hash;
	if (hashData.m_salt.size() == 0)
		hashData.m_salt.Assign(GetSaltData(ms_defaultSaltSize));
	hash.Update(hashData.m_salt, hashData.m_salt.size());
	hash.Update(hashData.m_data, hashData.m_data.size());
	hashData.m_data.CleanNew(hash.DIGESTSIZE);
	hash.Final(hashData.m_data);
}
CryptoPP::SecByteBlock SecureHelpers::GetSaltData(int amt)
{
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::SecByteBlock scratch(amt);
	rng.GenerateBlock(scratch, scratch.size());
	return scratch;
}
CryptoPP::SecByteBlock SecureHelpers::RSAEncrypt(
	const CryptoPP::SecByteBlock& data,
	CryptoPP::RSA::PublicKey& key)
{
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::RSAES_OAEP_SHA_Encryptor encryptor(key);
	if (encryptor.FixedMaxPlaintextLength() == 0)
	{
		/*encrption exception, error happened.*/
		int stop = 0;
	}
	if (data.size() > encryptor.FixedMaxPlaintextLength())
	{
		/*encrption exception, input data to big*/
		int stop = 0;
	}
	size_t ecl = encryptor.CiphertextLength(data.size());
	if (ecl == 0)
	{
		/*encryption error, ciphertextlenth not good*/
		int stop = 0;
	}
	CryptoPP::SecByteBlock crpytedData(ecl);
	encryptor.Encrypt(rng, data, data.size(), crpytedData);
	return crpytedData;
}

CryptoPP::SecByteBlock SecureHelpers::RSADecrypt(
	const CryptoPP::SecByteBlock& data,
	CryptoPP::RSA::PrivateKey & key)
{
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::RSAES_OAEP_SHA_Decryptor decryptor(key);
	if (decryptor.FixedMaxPlaintextLength() == 0)
	{
		/*encrption exception, error happened.*/
		int stop = 0;
	}
	if (data.size() > decryptor.FixedCiphertextLength())
	{
		/*encrption exception, input data to big*/
		int stop = 0;
	}
	size_t ecl = decryptor.MaxPlaintextLength(data.size());
	if (ecl == 0)
	{
		/*encryption error, ciphertextlenth not good*/
		int stop = 0;
	}
	CryptoPP::SecByteBlock recovered(ecl);
	try {
		CryptoPP::DecodingResult res =
			decryptor.Decrypt(rng, data, data.size(), recovered);
		recovered.resize(res.messageLength);
	}
	catch (const CryptoPP::Exception& e)
	{
		std::cout << e.what();
	}
	return recovered;
}

CryptoPP::SecByteBlock SecureHelpers::RSASHA256Sign(
	const CryptoPP::SecByteBlock & data,
	CryptoPP::RSA::PrivateKey & key)
{
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::RSASS<CryptoPP::PSS, CryptoPP::SHA512>::Signer signer(key);
	std::size_t length = signer.MaxSignatureLength();
	CryptoPP::SecByteBlock sig(length);
	length = signer.SignMessage(rng, data, data.size(), sig);
	sig.resize(length);
	return sig;
}

bool SecureHelpers::RSASHA256Verify(const CryptoPP::SecByteBlock & data,
	const CryptoPP::SecByteBlock& sig,
	CryptoPP::RSA::PublicKey & key)
{
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::RSASS<CryptoPP::PSS, CryptoPP::SHA512>::Verifier
		verifier(key);
	return verifier.VerifyMessage(data, data.size(), sig, sig.size());
}

void SecureHelpers::AESEncrypt(const AESData & data,
	CryptoPP::SecByteBlock & out)
{
	if (data.m_key.size() == 0)
		throw EncryptionException(EncryptionException::Code::BadKey);
	if (data.m_iv.size() == 0)
		throw EncryptionException(EncryptionException::Code::BadIv);
	if (data.m_data->size() == 0)
		throw EncryptionException(EncryptionException::Code::NoData);
	CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption encryptor(data.m_key,
		data.m_key.size(), data.m_iv);
	encryptor.ProcessData(out,*data.m_data, data.m_data->size());
}

void SecureHelpers::AESEncrypt(AESData& data)
{
	if (data.m_key.size() == 0)
		throw EncryptionException(EncryptionException::Code::BadKey);
	if (data.m_iv.size() == 0)
		throw EncryptionException(EncryptionException::Code::BadIv);
	if (data.m_data->size() == 0)
		throw EncryptionException(EncryptionException::Code::NoData);
	CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption encryptor(data.m_key,
		data.m_key.size(), data.m_iv);
	encryptor.ProcessData(*data.m_data, *data.m_data, data.m_data->size());
}

void SecureHelpers::AESDecrypt(AESData& data)
{
	CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption decryptor(data.m_key,
		data.m_key.size(), data.m_iv);
	decryptor.ProcessData(*data.m_data, *data.m_data, data.m_data->size());
}

CryptoPP::SecByteBlock SecureHelpers::MakeAESKey()
{
	CryptoPP::AutoSeededRandomPool rnd;
	CryptoPP::SecByteBlock key(0x00, ms_defaultAesKeySize);
	rnd.GenerateBlock(key, key.size());
	return key;
}

CryptoPP::SecByteBlock SecureHelpers::StringToSecBlock(const std::string & str)
{
	CryptoPP::SecByteBlock sb;
	sb.Assign((byte*)str.c_str(), str.size()+1);
	return sb;
}

CryptoPP::SecByteBlock SecureHelpers::MakeAESKey(const std::string & pass,
	const CryptoPP::SecByteBlock& salt)
{
	if (pass.size() == 0 || pass.size() > 32)
		throw EncryptionException(EncryptionException::Code::BadKey);
	HashData hash;
	hash.m_data.Assign((byte*)pass.c_str(), pass.size());
	hash.m_salt.Assign(salt);
	HashIt256(hash);
	return CopySecByteBlock(hash.m_data);
}

CryptoPP::SecByteBlock SecureHelpers::MakeAESIv()
{
	CryptoPP::AutoSeededRandomPool rnd;
	CryptoPP::SecByteBlock iv(ms_defaultIvSize);
	rnd.GenerateBlock(iv, iv.size());
	return iv;
}

KeyPair SecureHelpers::MakeRSAKeys(uint16_t size)
{
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::InvertibleRSAFunction params;
	params.GenerateRandomWithKeySize(rng, size);
	auto ret = KeyPair(params);
	ret.m_time =
		std::chrono::high_resolution_clock::now().time_since_epoch().count();
	return ret;
}

KeyPair SecureHelpers::MakeAndStoreRSAKeys(const std::string& path,
	uint16_t size,
	bool clobber)
{
	if (!clobber)
	{
		std::ifstream ifs(path);
		if (ifs.good())
			throw EncryptionException(EncryptionException::AlreadyExists);
	}
	auto keys = MakeRSAKeys(size);
	CryptoPP::ByteQueue bq;
	SerializeRSAKeys(keys, bq);
	CryptoPP::FileSink fs(path.c_str());
	bq.CopyTo(fs);
	fs.MessageEnd();
	return keys;
}

KeyPair SecureHelpers::LoadRSAKeys(const std::string & path)
{
	CryptoPP::ByteQueue bq;
	CryptoPP::FileSource fs(path.c_str(), true);
	fs.TransferTo(bq);
	KeyPair keys = DeserializeRSAKeys(bq);
	return keys;
}

void SecureHelpers::SerializeRSAKeys(KeyPair & keys,
	CryptoPP::ByteQueue& bq)
{
	bq.LazyPut((byte*)&keys.m_time, sizeof(keys.m_time));
	keys.m_private.Save(bq);
	keys.m_public.Save(bq);
}

KeyPair SecureHelpers::DeserializeRSAKeys(CryptoPP::ByteQueue & bq)
{
	KeyPair keys;
	bq.Get((byte*)&keys.m_time, sizeof(keys.m_time));
	keys.m_private.Load(bq);
	keys.m_public.Load(bq);
	return keys;
}

bool SecureHelpers::ValidateRSAKeys(KeyPair & keys)
{
	bool together = keys.m_private.GetModulus() == keys.m_public.GetModulus();
	CryptoPP::AutoSeededRandomPool rng;
	return keys.m_private.Validate(rng, 3)
		&& keys.m_public.Validate(rng, 3)
		&& together;
}



}



