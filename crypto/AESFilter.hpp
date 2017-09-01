#pragma once
#include "SecureHelpers.hpp"
#include "../Filter.hpp"

namespace cg {
/**A reader/writing filter for AES encryption.*/
class AESEncryptFilter : public cg::Filter
{
public:
	/**always returns false because the size will never change.*/
	virtual bool SizeChanges() const
	{
		return false;
	}
	/**Create it.
	\param key The key for encryption
	\param iv The IV for encryption*/
	AESEncryptFilter(const CryptoPP::SecByteBlock& key,
		const CryptoPP::SecByteBlock& iv)
		:m_key(key), m_iv(iv) {}
	/**Transform data in place (no copies).
	\param data The data place.
	\param size The data size.*/
	virtual void Transform(char* data, std::size_t size)
	{
		cg::SecureHelpers::AESEncrypt(data, data, size, size, m_key, m_iv);
	}
private:
	/**The key used.*/
	const CryptoPP::SecByteBlock& m_key;
	/**The IV used.*/
	const CryptoPP::SecByteBlock& m_iv;
};

/**A reader/writing filter for AES decryption.*/
class AESDecryptFilter : public cg::Filter
{
public:
	/**always returns false because the size will never change.*/
	virtual bool SizeChanges() const
	{
		return false;
	}
	/**Create it.
	\param key The key for decryption
	\param iv The IV for decryption*/
	AESDecryptFilter(const CryptoPP::SecByteBlock& key,
		const CryptoPP::SecByteBlock& iv)
		:m_key(key), m_iv(iv) {}
	/**Transform data in place (no copies).
	\param data The data place.
	\param size The data size.*/
	virtual void Transform(char* data, std::size_t size)
	{
		cg::SecureHelpers::AESDecrypt(data, data, size, size, m_key, m_iv);
	}
private:
	/**The key used.*/
	const CryptoPP::SecByteBlock& m_key;
	/**The IV used.*/
	const CryptoPP::SecByteBlock& m_iv;

};


}