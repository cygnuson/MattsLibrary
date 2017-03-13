#pragma once

#include <fstream>
#include <string>

#include <cryptopp/sha.h>
#include <cryptopp/files.h>
#include <cryptopp/rsa.h>
#include <cryptopp/modes.h>
#include <cryptopp/pssr.h>
#include <cryptopp/osrng.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/secblock.h>

#include "../exception.hpp"
#include "../Memory.hpp"


namespace cg {

class EncryptionException : public cg::Exception
{
public:
	/**The exception codes.*/
	enum Code
	{
		/**Bad IV*/
		BadIv,
		/**BadKey*/
		BadKey,
		/**No data given to be encrypted.*/
		NoData,
		/**The place to store data already exists.*/
		AlreadyExists,
		/**Destination to small*/
		BadMem,

	};
	/**Create the Exception
	\param code The code issue.*/
	EncryptionException(Code code)
		:m_code(code) {};
	/**Get the message.
	\return A const char* with the message.*/
	virtual std::string What() const override;
	virtual std::string ToString() const override;
private:
	/**The message code.*/
	Code m_code;

};

/**The result of  hash operation.*/
struct HashData
{
	/**The hash itself*/
	CryptoPP::SecByteBlock m_data;
	/**The salt that was used to calculate the hash.*/
	CryptoPP::SecByteBlock m_salt;
};

/**The result of an AES encryption and key gen*/
struct AESData
{
	/**Empty aes data with a new byte block*/
	AESData() :m_data(cg::New<CryptoPP::SecByteBlock>())
	{
		m_created = true;
	};
	/**Copy ctor
	\param other the thing to move.*/
	AESData(AESData&& other)
		:m_data(std::move(other.m_data)),
		m_key(std::move(other.m_key)),
		m_iv(std::move(other.m_iv))
	{
		m_created = other.m_created;
		other.m_created = false;
	}
	/**Make aesdata with a reference to an already existing byte block.*/
	AESData(CryptoPP::SecByteBlock& data) :m_data(&data) {};
	/**Dextroy the data if it was creatd.*/
	~AESData()
	{
		if (m_created) 
			cg::DeleteA(m_data);
	}
	/**The data.*/
	CryptoPP::SecByteBlock* m_data;
	/**The key used.*/
	CryptoPP::SecByteBlock m_key;
	/**The IV used.*/
	CryptoPP::SecByteBlock m_iv;
private:
	bool m_created = false;
};

/**A pair of RSA keys.*/
struct KeyPair
{
	/**Create an empty key pair.*/
	KeyPair() {};
	/**Create a key pair.
	\param func The invertibe function.*/
	KeyPair(CryptoPP::InvertibleRSAFunction& func)
		:m_private(func), m_public(func) {};
	/**The private key.*/
	CryptoPP::RSA::PrivateKey m_private;
	/**The public key.*/
	CryptoPP::RSA::PublicKey m_public;
	/**The creation time of the keys as time sense epoch.*/
	uint64_t m_time = 0;
};

class SecureHelpers
{
public:
	/**Copy a SecByteBlock
	\param b The block to copy.
	\return A new SecByteBlock which owns its data.*/
	static CryptoPP::SecByteBlock CopySecByteBlock(
		const CryptoPP::SecByteBlock& b);
	/**Hash some data.
	\param hashData A HashData with the salt and data to be hasehd. If the salt
	is empty, it will be generated with the default size.*/
	static void HashIt512(HashData& hashData);
	/**Hash some data.
	\param hashData A HashData with the salt and data to be hasehd. If the salt
	is empty, it will be generated with the default size.*/
	static void HashIt256(HashData& hashData);
	/**Get salty.
	\param amt The amount of bytes to make the salt.
	\return A secByteBlock That is the salt.*/
	static CryptoPP::SecByteBlock GetSaltData(int amt);
	/**Encrypt some data.
	\param data The data to encrypt.
	\param key The PublicKey that will be used to encrypt the data.
	\return The processed data.*/
	static CryptoPP::SecByteBlock RSAEncrypt(
		const CryptoPP::SecByteBlock& data,
		CryptoPP::RSA::PublicKey& key);
	/**Encrypt some data.
	\param data The data to encrypt.
	\param size The size of the data.
	\param key The PublicKey that will be used to encrypt the data.
	\return The processed data.*/
	static CryptoPP::SecByteBlock RSAEncrypt(
		const char* data,
		std::size_t size,
		CryptoPP::RSA::PublicKey& key);
	/**Dencrypt some data.
	\param data The data to decrypt.
	\param key The PrivateKey that will be used to decrypt the data.
	\return The processed data.*/
	static CryptoPP::SecByteBlock RSADecrypt(
		const CryptoPP::SecByteBlock& data,
		CryptoPP::RSA::PrivateKey& key);
	/**Dencrypt some data.
	\param data The data to decrypt.
	\param size The size of the data.
	\param key The PrivateKey that will be used to decrypt the data.
	\return The processed data.*/
	static CryptoPP::SecByteBlock RSADecrypt(
		const char* data,
		std::size_t size,
		CryptoPP::RSA::PrivateKey& key);
	/**Sign some data.
	\param data The data to sign.
	\param key The PrivateKey that will be used to sign the data.
	\return The processed data.*/
	static CryptoPP::SecByteBlock RSASHA256Sign(
		const CryptoPP::SecByteBlock& data,
		CryptoPP::RSA::PrivateKey& key);
	/**Verify some data.
	\param data The data to Verify.
	\param key The PublicKey that will be used to Verify the data.
	\param sig The signature to verify.
	\return True if the data was verified.*/
	static bool RSASHA256Verify(
		const CryptoPP::SecByteBlock& data,
		const CryptoPP::SecByteBlock& sig,
		CryptoPP::RSA::PublicKey& key);
	/**Encrypt with AES. If the key or IV is empty, they will be generated and
	placed into the data object with the max key and Iv size.
	\param data An AESData object with a key, IV, and data.  THe data will be
	overwritten with the new data.
	\param out The place to put the output.*/
	static void AESEncrypt(const AESData& data,
		CryptoPP::SecByteBlock& out);
	/**Encrypt with AES.
	\param dest The place to put the data.
	\param src The input data.
	\param dSize The destination size.
	\param sSize The src size.
	\param key The key.
	\param iv The iv to use.*/
	static void AESEncrypt(char* dest,
		const char* src,
		std::size_t dSize,
		std::size_t sSize,
		const CryptoPP::SecByteBlock& key,
		const CryptoPP::SecByteBlock& iv);
	/**Encrypt with AES. If the key or IV is empty, they will be generated and
	placed into the data object with the max key and Iv size.
	\param An AESData object with a key, IV, and data.  THe data will be
	overwritten with the new data.*/
	static void AESEncrypt(AESData& data);
	/**Decrypt with AES.
	\param An AESData object with a key, IV, and data.  THe data will be
	overwritten with the new data.*/
	static void AESDecrypt(AESData& data);
	/**Decrypt with AES.
		\param dest The place to put the data.
	\param src The input data.
	\param dSize The destination size.
	\param sSize The src size.
	\param key The key.
	\param iv The iv to use.*/
	static void AESDecrypt(char* dest,
		const char* src,
		std::size_t dSize,
		std::size_t sSize,
		const CryptoPP::SecByteBlock& key,
		const CryptoPP::SecByteBlock& iv);
	/**Generate a random AES key with the max key length.
	\return The key in the form of a secbyteblock.*/
	static CryptoPP::SecByteBlock MakeAESKey();
	/**Create a sec byte block from a string.
	\param str The string to make into a secbyteblock.
	\return A SecByteBlock that is equal to the string supplied.*/
	static CryptoPP::SecByteBlock StringToSecBlock(const std::string& str);
	/**Generate a random AES key with the max key length.
	\param pass A user string that will be padded to the key size.  pass must
	be 1<=[pass]=<32
	\return The key in the form of a secbyteblock.*/
	static CryptoPP::SecByteBlock MakeAESKey(const std::string& pass,
		const CryptoPP::SecByteBlock& salt);
	/**Generate a random AES IV.
	\return The IV in the form of a secbyteblock.*/
	static CryptoPP::SecByteBlock MakeAESIv();
	/**Create an RSA key. A pair of keys.
	\return The pair of keys*/
	static KeyPair MakeRSAKeys(uint16_t size = ms_defaultRSASize);
	/**Create and store RSA keys.
	\param path The path to save the keys.
	\param size The size of the keys to make.
	\param clobber True to overwrite if it exists.
	\throw EncryptionException with m_code as Code::AlreadyExists if clobber
	is false and the file exists.
	\return The pair of keys*/
	static KeyPair MakeAndStoreRSAKeys(const std::string& path,
		uint16_t size,
		bool clobber);
	/**Load some RSA keys froma file.
	\param path The path to load from.
	\return The pair of keys*/
	static KeyPair LoadRSAKeys(const std::string& path);
	/**Serialize some RSA keys.
	\param keys A KeyPair of the keys to serialize.
	\param bq The ByteQueue to put the keys into.*/
	static void SerializeRSAKeys(KeyPair& keys,
		CryptoPP::ByteQueue& bq);
	/**Deserialize some RSA keys.
	\param bytes The ByteQueue with the data.
	\return A KeyPair with the keys.*/
	static KeyPair DeserializeRSAKeys(CryptoPP::ByteQueue& bytes);
	/**Validate loaded keys.
	\param keys A KeyPair to be validated.*/
	static bool ValidateRSAKeys(KeyPair& keys);

	/**The default salt amount.*/
	const static int ms_defaultSaltSize = 16;
	/**The default aes key size.*/
	const static int ms_defaultAesKeySize = 32;
	/**The default iv size*/
	const static int ms_defaultIvSize = 16;
	/**Default rsa size*/
	const static int ms_defaultRSASize = 2048;
	/**The cipher text size for an rsa with default values.*/
	const static int ms_defaultRSACipherSize = (ms_defaultRSASize / 8);
};

}


