﻿// validat7.cpp - originally written and placed in the public domain by Wei Dai
//                CryptoPP::Test namespace added by JW in February 2017.
//                Source files split in July 2018 to expedite compiles.

#include "pch.h"

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "cryptlib.h"
#include "cpu.h"
#include "validate.h"

#include "asn.h"
#include "oids.h"

#include "sha.h"
#include "sha3.h"

#include "dh.h"
#include "luc.h"
#include "mqv.h"
#include "xtr.h"
#include "hmqv.h"
#include "pubkey.h"
#include "xtrcrypt.h"
#include "eccrypto.h"

// Curve25519
#include "xed25519.h"
#include "donna.h"
#include "naclite.h"

#include <iostream>
#include <iomanip>
#include <sstream>

// Aggressive stack checking with VS2005 SP1 and above.
#if (_MSC_FULL_VER >= 140050727)
# pragma strict_gs_check (on)
#endif

#if CRYPTOPP_MSC_VERSION
# pragma warning(disable: 4505 4355)
#endif

NAMESPACE_BEGIN(CryptoPP)
NAMESPACE_BEGIN(Test)

ANONYMOUS_NAMESPACE_BEGIN

inline bool operator==(const x25519& lhs, const x25519& rhs)
{
	// This is a hack because the KeyAgreement classes do not make it easy to access the PrivateKey
	ByteQueue q1, q2;
	lhs.DEREncodePrivateKey(q1);
	rhs.DEREncodePrivateKey(q2);

	return q1 == q2;
}

inline bool operator!=(const x25519& lhs, const x25519& rhs)
{
	return !operator==(lhs, rhs);
}

ANONYMOUS_NAMESPACE_END

bool ValidateDH()
{
	std::cout << "\nDH validation suite running...\n\n";

	FileSource f(DataDir("TestData/dh1024.dat").c_str(), true, new HexDecoder);
	DH dh(f);
	return SimpleKeyAgreementValidate(dh);
}

bool ValidateX25519()
{
	std::cout << "\nx25519 validation suite running...\n\n";

	FileSource f(DataDir("TestData/x25519.dat").c_str(), true, new HexDecoder);
	x25519 dh(f);
	return SimpleKeyAgreementValidate(dh);
}

bool ValidateMQV()
{
	std::cout << "\nMQV validation suite running...\n\n";

	FileSource f(DataDir("TestData/mqv1024.dat").c_str(), true, new HexDecoder);
	MQV mqv(f);
	return AuthenticatedKeyAgreementValidate(mqv);
}

bool ValidateHMQV()
{
	std::cout << "\nHMQV validation suite running...\n\n";
	bool success = true, fail;

	FileSource f256(DataDir("TestData/hmqv256.dat").c_str(), true, new HexDecoder);
	FileSource f384(DataDir("TestData/hmqv384.dat").c_str(), true, new HexDecoder);
	FileSource f512(DataDir("TestData/hmqv512.dat").c_str(), true, new HexDecoder);

	/////////////////////////

	std::cout << "HMQV with NIST P-256 and SHA-256:" << std::endl;

	ECHMQV256 hmqvB256(false);
	hmqvB256.AccessGroupParameters().BERDecode(f256);
	const OID oid = ASN1::secp256r1();
	ECHMQV< ECP >::Domain hmqvA256(oid, true /*client*/);

	fail = !AuthenticatedKeyAgreementWithRolesValidate(hmqvA256, hmqvB256);
	success = !fail && success;
	if (fail == false)
		std::cout << "passed    authenticated key agreement" << std::endl;
	else
		std::cout << "FAILED    authenticated key agreement" << std::endl;

	/////////////////////////

	std::cout << "HMQV with NIST P-384 and SHA-384:" << std::endl;

	ECHMQV384 hmqvB384(false);
	hmqvB384.AccessGroupParameters().BERDecode(f384);
	const OID oid384 = ASN1::secp384r1();
	ECHMQV384 hmqvA384(oid384, true /*client*/);

	fail = !AuthenticatedKeyAgreementWithRolesValidate(hmqvA384, hmqvB384);
	success = !fail && success;
	if (fail == false)
		std::cout << "passed    authenticated key agreement" << std::endl;
	else
		std::cout << "FAILED    authenticated key agreement" << std::endl;

	/////////////////////////

	std::cout << "HMQV with NIST P-521 and SHA-512:" << std::endl;

	ECHMQV512 hmqvB521(false);
	hmqvB521.AccessGroupParameters().BERDecode(f512);
	const OID oid521 = ASN1::secp521r1();
	ECHMQV512 hmqvA521(oid521, true /*client*/);

	fail = !AuthenticatedKeyAgreementWithRolesValidate(hmqvA521, hmqvB521);
	success = !fail && success;
	if (fail == false)
		std::cout << "passed    authenticated key agreement" << std::endl;
	else
		std::cout << "FAILED    authenticated key agreement" << std::endl;

	return success;
}

bool ValidateFHMQV()
{
	std::cout << "\nFHMQV validation suite running...\n\n";
	bool success = true, fail;

	FileSource f256(DataDir("TestData/fhmqv256.dat").c_str(), true, new HexDecoder);
	FileSource f384(DataDir("TestData/fhmqv384.dat").c_str(), true, new HexDecoder);
	FileSource f512(DataDir("TestData/fhmqv512.dat").c_str(), true, new HexDecoder);

	/////////////////////////

	std::cout << "FHMQV with NIST P-256 and SHA-256:" << std::endl;

	ECFHMQV256 fhmqvB256(false);
	fhmqvB256.AccessGroupParameters().BERDecode(f256);
	const OID oid = ASN1::secp256r1();
	ECFHMQV< ECP >::Domain fhmqvA256(oid, true /*client*/);

	fail = !AuthenticatedKeyAgreementWithRolesValidate(fhmqvA256, fhmqvB256);
	success = !fail && success;
	if (fail == false)
		std::cout << "passed    authenticated key agreement" << std::endl;
	else
		std::cout << "FAILED    authenticated key agreement" << std::endl;

	/////////////////////////

	std::cout << "FHMQV with NIST P-384 and SHA-384:" << std::endl;

	ECHMQV384 fhmqvB384(false);
	fhmqvB384.AccessGroupParameters().BERDecode(f384);
	const OID oid384 = ASN1::secp384r1();
	ECHMQV384 fhmqvA384(oid384, true /*client*/);

	fail = !AuthenticatedKeyAgreementWithRolesValidate(fhmqvA384, fhmqvB384);
	success = !fail && success;
	if (fail == false)
		std::cout << "passed    authenticated key agreement" << std::endl;
	else
		std::cout << "FAILED    authenticated key agreement" << std::endl;

	/////////////////////////

	std::cout << "FHMQV with NIST P-521 and SHA-512:" << std::endl;

	ECHMQV512 fhmqvB521(false);
	fhmqvB521.AccessGroupParameters().BERDecode(f512);
	const OID oid521 = ASN1::secp521r1();
	ECHMQV512 fhmqvA521(oid521, true /*client*/);

	fail = !AuthenticatedKeyAgreementWithRolesValidate(fhmqvA521, fhmqvB521);
	success = !fail && success;
	if (fail == false)
		std::cout << "passed    authenticated key agreement" << std::endl;
	else
		std::cout << "FAILED    authenticated key agreement" << std::endl;

	return success;
}

bool ValidateLUC_DH()
{
	std::cout << "\nLUC-DH validation suite running...\n\n";

	FileSource f(DataDir("TestData/lucd512.dat").c_str(), true, new HexDecoder);
	LUC_DH dh(f);
	return SimpleKeyAgreementValidate(dh);
}

bool ValidateXTR_DH()
{
	std::cout << "\nXTR-DH validation suite running...\n\n";

	FileSource f(DataDir("TestData/xtrdh171.dat").c_str(), true, new HexDecoder);
	XTR_DH dh(f);
	return SimpleKeyAgreementValidate(dh);
}

bool ValidateECP_Agreement()
{
	ECDH<ECP>::Domain ecdhc(ASN1::secp192r1());
	ECMQV<ECP>::Domain ecmqvc(ASN1::secp192r1());
	bool pass = SimpleKeyAgreementValidate(ecdhc);
	pass = AuthenticatedKeyAgreementValidate(ecmqvc) && pass;

	std::cout << "Turning on point compression..." << std::endl;
	ecdhc.AccessGroupParameters().SetPointCompression(true);
	ecmqvc.AccessGroupParameters().SetPointCompression(true);
	pass = SimpleKeyAgreementValidate(ecdhc) && pass;
	pass = AuthenticatedKeyAgreementValidate(ecmqvc) && pass;

	return pass;
}

bool ValidateEC2N_Agreement()
{
	ECDH<EC2N>::Domain ecdhc(ASN1::sect193r1());
	ECMQV<EC2N>::Domain ecmqvc(ASN1::sect193r1());
	bool pass = SimpleKeyAgreementValidate(ecdhc);
	pass = AuthenticatedKeyAgreementValidate(ecmqvc) && pass;

	std::cout << "Turning on point compression..." << std::endl;
	ecdhc.AccessGroupParameters().SetPointCompression(true);
	ecmqvc.AccessGroupParameters().SetPointCompression(true);
	pass = SimpleKeyAgreementValidate(ecdhc) && pass;
	pass = AuthenticatedKeyAgreementValidate(ecmqvc) && pass;

	return pass;
}

// TestX25519 is slighty more comprehensive than ValidateX25519
// because it cross-validates against Bernstein's NaCL library.
// TestX25519 called in Debug builds.
bool TestX25519()
{
	std::cout << "\nTesting curve25519 Key Agreements...\n\n";
	const unsigned int AGREE_COUNT = 64;
	bool pass = true, fail;

	size_t i = 0;
	unsigned int failed = 0;

	SecByteBlock priv1(32), priv2(32), pub1(32), pub2(32), share1(32), share2(32);
	for (i=0, failed=0; i<AGREE_COUNT; ++i)
	{
		GlobalRNG().GenerateBlock(priv1, priv1.size());
		GlobalRNG().GenerateBlock(priv2, priv2.size());

		priv1[0] &= 248; priv1[31] &= 127; priv1[31] |= 64;
		priv2[0] &= 248; priv2[31] &= 127; priv2[31] |= 64;

		// Andrew Moon's curve25519-donna
		Donna::curve25519_mult(pub1, priv1);
		Donna::curve25519_mult(pub2, priv2);

		int ret1 = Donna::curve25519_mult(share1, priv1, pub2);
		int ret2 = Donna::curve25519_mult(share2, priv2, pub1);
		int ret3 = std::memcmp(share1, share2, 32);

#if defined(CRYPTOPP_DISABLE_NACL)
		int ret4=0, ret5=0, ret6=0;
#else
		// Bernstein's NaCl requires DefaultAutoSeededRNG.
		NaCl::crypto_box_keypair(pub2, priv2);

		int ret4 = Donna::curve25519_mult(share1, priv1, pub2);
		int ret5 = NaCl::crypto_scalarmult(share2, priv2, pub1);
		int ret6 = std::memcmp(share1, share2, 32);
#endif

		fail = ret1 != 0 || ret2 != 0 || ret3 != 0 || ret4 != 0 || ret5 != 0 || ret6 != 0;
		if (fail) failed++;
	}

	failed ? fail = true : fail = false;
	pass = pass && !fail;

	std::cout << (fail ? "FAILED" : "passed") << "  ";
	std::cout << AGREE_COUNT << " key agreements" << std::endl;

	try {

		FileSource f1(DataDir("TestData/x25519.dat").c_str(), true, new HexDecoder);
		FileSource f2(DataDir("TestData/x25519v0.dat").c_str(), true, new HexDecoder);
		FileSource f3(DataDir("TestData/x25519v1.dat").c_str(), true, new HexDecoder);

		x25519 x1(f1);
		x25519 x2(f2);
		x25519 x3(f3);

		FileSource f4(DataDir("TestData/x25519.dat").c_str(), true, new HexDecoder);
		FileSource f5(DataDir("TestData/x25519v0.dat").c_str(), true, new HexDecoder);
		FileSource f6(DataDir("TestData/x25519v1.dat").c_str(), true, new HexDecoder);

		x1.Load(f4);
		x2.Load(f5);
		x3.Load(f6);

		// No throw is success
		fail = false;
	}
	catch (const BERDecodeErr&) {
		fail = true;
	}

	pass = pass && !fail;

	std::cout << (fail ? "FAILED" : "passed") << "  ";
	std::cout << "x25519 load and save\n";

#if defined(CRYPTOPP_EXTENDED_VALIDATION)
	{
		x25519 x1(GlobalRNG()), x2;

		ByteQueue q;
		x1.DEREncode(q);
		x2.BERDecode(q);

		fail = (x1 != x2);
		pass = pass && !fail;

		std::cout << (fail ? "FAILED" : "passed") << "  ";
		std::cout << "x25519 encoding and decoding\n";
	}
	{
		SecByteBlock b1(32);
		GlobalRNG().GenerateBlock(b1, b1.size());
		b1[0] &= 248; b1[31] &= 127; b1[31] |= 64;

		try {
			fail = false;
			x25519 x1(b1);
			if (!x1.Validate(GlobalRNG(), 3))
				throw Exception(Exception::OTHER_ERROR, "x25519::Validate");
		} catch(const Exception&) {
			fail = true;
		}

		pass = pass && !fail;

		std::cout << (fail ? "FAILED" : "passed") << "  ";
		std::cout << "x25519 private key byte buffer construction\n";

		std::reverse(b1.begin(), b1.begin()+b1.size());
		Integer x(b1.begin(), b1.size());

		try {
			fail = false;
			x25519 x1(x);
			if (!x1.Validate(GlobalRNG(), 3))
				throw Exception(Exception::OTHER_ERROR, "x25519::Validate");
		} catch(const Exception&) {
			fail = true;
		}

		pass = pass && !fail;

		std::cout << (fail ? "FAILED" : "passed") << "  ";
		std::cout << "x25519 private key Integer construction\n";
	}

	{
		SecByteBlock b1(32), b2(32);
		GlobalRNG().GenerateBlock(b1, b1.size());
		b1[0] &= 248; b1[31] &= 127; b1[31] |= 64;
		Donna::curve25519_mult(b2, b1);

		try {
			fail = false;
			x25519 x1(b2, b1);
			if (!x1.Validate(GlobalRNG(), 3))
				throw Exception(Exception::OTHER_ERROR, "x25519::Validate");
		} catch(const Exception&) {
			fail = true;
		}

		pass = pass && !fail;

		std::cout << (fail ? "FAILED" : "passed") << "  ";
		std::cout << "x25519 public key byte buffer construction\n";

		SecByteBlock b3(b1), b4(b2);
		std::reverse(b3.begin(), b3+b3.size());
		std::reverse(b4.begin(), b4+b4.size());

		Integer x(b3, b3.size());
		Integer y(b4, b4.size());

		try {
			fail = false;
			x25519 x1(y, x);
			if (!x1.Validate(GlobalRNG(), 3))
				throw Exception(Exception::OTHER_ERROR, "x25519::Validate");
		} catch(const Exception&) {
			fail = true;
		}

		pass = pass && !fail;

		std::cout << (fail ? "FAILED" : "passed");
		std::cout << "  " << "x25519 public key Integer construction\n";

		try {
			fail = false;
			x25519 x1(b2, b1);
			x25519 x2(y, x);
			if (x1 != x2)
				throw Exception(Exception::OTHER_ERROR, "x25519::Validate");
		} catch(const Exception&) {
			fail = true;
		}

		pass = pass && !fail;

		std::cout << (fail ? "FAILED" : "passed") << "  ";
		std::cout << "x25519 byte buffer and Integer consistency\n";
	}
#endif

	return pass;
}

// TestEd25519 is slighty more comprehensive than ValidateEd25519
// because it cross-validates against Bernstein's NaCL library.
// TestEd25519 called in Debug builds.
bool TestEd25519()
{
	std::cout << "\nTesting ed25519 Signatures...\n\n";
	bool pass = true, fail;

	size_t i = 0;
	unsigned int failed = 0;

#ifndef CRYPTOPP_DISABLE_NACL
	const unsigned int SIGN_COUNT = 64, MSG_SIZE=128;
	const unsigned int NACL_EXTRA=NaCl::crypto_sign_BYTES;

	// Test key conversion
	byte seed[32], sk1[64], sk2[64], pk1[32], pk2[32];
	for (i = 0, failed = 0; i<SIGN_COUNT; ++i)
	{
		GlobalRNG().GenerateBlock(seed, 32);
		std::memcpy(sk1, seed, 32);
		std::memcpy(sk2, seed, 32);

		int ret1 = NaCl::crypto_sign_sk2pk(pk1, sk1);
		int ret2 = Donna::ed25519_publickey(pk2, sk2);
		int ret3 = std::memcmp(pk1, pk2, 32);

		fail = ret1 != 0 || ret2 != 0 || ret3 != 0;
		if (fail) failed++;
	}

	failed ? fail = true : fail = false;
	pass = pass && !fail;

	std::cout << (fail ? "FAILED" : "passed") << "  ";
	std::cout << SIGN_COUNT << " public keys" << std::endl;

	// Test signature generation
	for (i = 0, failed = 0; i<SIGN_COUNT; ++i)
	{
		// Fresh keypair
		(void)NaCl::crypto_sign_keypair(pk1, sk1);
		std::memcpy(sk2, sk1, 32);
		std::memcpy(pk2, pk1, 32);

		// Message and signatures
		byte msg[MSG_SIZE], sig1[MSG_SIZE+NACL_EXTRA], sig2[64];
		GlobalRNG().GenerateBlock(msg, MSG_SIZE);
		size_t len = GlobalRNG().GenerateWord32(0, MSG_SIZE);

		// Spike the signatures
		sig1[1] = 1; sig2[2] = 2;
		word64 smlen = sizeof(sig1);

		int ret1 = NaCl::crypto_sign(sig1, &smlen, msg, len, sk1);
		int ret2 = Donna::ed25519_sign(msg, len, sk2, pk2, sig2);
		int ret3 = std::memcmp(sig1, sig2, 64);

		fail = ret1 != 0 || ret2 != 0 || ret3 != 0;
		if (fail) failed++;
	}

	failed ? fail = true : fail = false;
	pass = pass && !fail;

	std::cout << (fail ? "FAILED" : "passed") << "  ";
	std::cout << SIGN_COUNT << " signatures" << std::endl;

	// Test signature verification
	for (i = 0, failed = 0; i<SIGN_COUNT; ++i)
	{
		// Fresh keypair
		(void)NaCl::crypto_sign_keypair(pk1, sk1);
		std::memcpy(sk2, sk1, 32);
		std::memcpy(pk2, pk1, 32);

		// Message and signatures
		byte msg1[MSG_SIZE+NACL_EXTRA], msg2[MSG_SIZE];
		byte sig1[MSG_SIZE+NACL_EXTRA], sig2[64];
		GlobalRNG().GenerateBlock(msg1, MSG_SIZE);
		size_t len = GlobalRNG().GenerateWord32(0, MSG_SIZE);
		std::memcpy(msg2, msg1, len);

		// Spike the signatures
		sig1[1] = 1; sig2[2] = 2;

		word64 smlen = sizeof(sig1);
		int ret1 = NaCl::crypto_sign(sig1, &smlen, msg1, len, sk1);
		int ret2 = Donna::ed25519_sign(msg2, len, sk2, pk2, sig2);
		int ret3 = std::memcmp(sig1, sig2, 64);

		bool tamper = !!GlobalRNG().GenerateBit();
		if (tamper)
		{
			sig1[1] ^= 1;
			sig2[1] ^= 1;
		}

		// Verify the other's signature using the other's key
		word64 mlen = len+NACL_EXTRA;
		int ret4 = NaCl::crypto_sign_open(msg1, &mlen, sig1, smlen, pk2);
		int ret5 = Donna::ed25519_sign_open(msg2, len, pk1, sig2);

		fail = ret1 != 0 || ret2 != 0 || ret3 != 0 || ((ret4 != 0) ^ tamper) || ((ret5 != 0) ^ tamper);
		if (fail) failed++;
	}

	failed ? fail = true : fail = false;
	pass = pass && !fail;

	std::cout << (fail ? "FAILED" : "passed") << "  ";
	std::cout << SIGN_COUNT << " verifications" << std::endl;

	// Test signature verification using streams
	for (i = 0, failed = 0; i<SIGN_COUNT; ++i)
	{
		// Fresh keypair
		(void)NaCl::crypto_sign_keypair(pk1, sk1);
		std::memcpy(sk2, sk1, 32);
		std::memcpy(pk2, pk1, 32);

		// Message and signatures
		byte msg1[MSG_SIZE+NACL_EXTRA], msg2[MSG_SIZE];
		byte sig1[MSG_SIZE+NACL_EXTRA], sig2[64];
		GlobalRNG().GenerateBlock(msg1, MSG_SIZE);
		size_t len = GlobalRNG().GenerateWord32(0, MSG_SIZE);
		std::memcpy(msg2, msg1, len);

		// Spike the signatures
		sig1[1] = 1; sig2[2] = 2;

		// Create a stream
		std::string str2((const char*)msg2, len);
		std::istringstream iss(str2);

		word64 smlen = sizeof(sig1);
		int ret1 = NaCl::crypto_sign(sig1, &smlen, msg1, len, sk1);
		int ret2 = Donna::ed25519_sign(iss, sk2, pk2, sig2);
		int ret3 = std::memcmp(sig1, sig2, 64);

		bool tamper = !!GlobalRNG().GenerateBit();
		if (tamper)
		{
			sig1[1] ^= 1;
			sig2[1] ^= 1;
		}

		// Reset stream
		iss.clear();
		iss.seekg(0);

		// Verify the other's signature using the other's key
		word64 mlen = len+NACL_EXTRA;
		int ret4 = NaCl::crypto_sign_open(msg1, &mlen, sig1, smlen, pk2);
		int ret5 = Donna::ed25519_sign_open(iss, pk1, sig2);

		fail = ret1 != 0 || ret2 != 0 || ret3 != 0 || ((ret4 != 0) ^ tamper) || ((ret5 != 0) ^ tamper);
		if (fail) failed++;
	}

	failed ? fail = true : fail = false;
	pass = pass && !fail;

	std::cout << (fail ? "FAILED" : "passed") << "  ";
	std::cout << SIGN_COUNT << " streams" << std::endl;
#endif

	// RFC 8032 test vector
	try
	{
		// RFC 8032 Ed25519 test vector 3, p. 23
		byte sk[] = {
			0xc5,0xaa,0x8d,0xf4,0x3f,0x9f,0x83,0x7b,0xed,0xb7,0x44,0x2f,0x31,0xdc,0xb7,0xb1,
			0x66,0xd3,0x85,0x35,0x07,0x6f,0x09,0x4b,0x85,0xce,0x3a,0x2e,0x0b,0x44,0x58,0xf7
		};
		byte pk[] = {
			0xfc,0x51,0xcd,0x8e,0x62,0x18,0xa1,0xa3,0x8d,0xa4,0x7e,0xd0,0x02,0x30,0xf0,0x58,
			0x08,0x16,0xed,0x13,0xba,0x33,0x03,0xac,0x5d,0xeb,0x91,0x15,0x48,0x90,0x80,0x25
		};

		const byte exp[] = {
			0x62,0x91,0xd6,0x57,0xde,0xec,0x24,0x02,0x48,0x27,0xe6,0x9c,0x3a,0xbe,0x01,0xa3,
			0x0c,0xe5,0x48,0xa2,0x84,0x74,0x3a,0x44,0x5e,0x36,0x80,0xd7,0xdb,0x5a,0xc3,0xac,
			0x18,0xff,0x9b,0x53,0x8d,0x16,0xf2,0x90,0xae,0x67,0xf7,0x60,0x98,0x4d,0xc6,0x59,
			0x4a,0x7c,0x15,0xe9,0x71,0x6e,0xd2,0x8d,0xc0,0x27,0xbe,0xce,0xea,0x1e,0xc4,0x0a
		};

		const byte msg[2] = {0xaf, 0x82}; byte sig[64];

		// Test the filter framework
		ed25519Signer signer(pk, sk);
		StringSource(msg, sizeof(msg), true, new SignerFilter(NullRNG(), signer, new ArraySink(sig, sizeof(sig))));

		if (std::memcmp(exp, sig, 64) != 0)
			throw Exception(Exception::OTHER_ERROR, "TestEd25519: SignerFilter");

		ed25519Verifier verifier(pk);
		int flags = SignatureVerificationFilter::THROW_EXCEPTION | SignatureVerificationFilter::SIGNATURE_AT_END;
		std::string msg_sig = std::string((char*)msg, sizeof(msg)) + std::string((char*)sig, sizeof(sig));
		StringSource(msg_sig, true, new SignatureVerificationFilter(verifier, NULLPTR, flags));

		// No throw is success
		fail = false;
	}
	catch(const Exception&)
	{
		fail = true;
	}

	pass = pass && !fail;

	std::cout << (fail ? "FAILED" : "passed") << "  ";
	std::cout << "RFC 8032 test vectors" << std::endl;

	// Test key loads
	try {
		FileSource f1(DataDir("TestData/ed25519.dat").c_str(), true, new HexDecoder);
		FileSource f2(DataDir("TestData/ed25519v0.dat").c_str(), true, new HexDecoder);
		FileSource f3(DataDir("TestData/ed25519v1.dat").c_str(), true, new HexDecoder);

		ed25519::Signer s1(f1);
		ed25519::Signer s2(f2);
		ed25519::Signer s3(f3);

		FileSource f4(DataDir("TestData/ed25519.dat").c_str(), true, new HexDecoder);
		FileSource f5(DataDir("TestData/ed25519v0.dat").c_str(), true, new HexDecoder);
		FileSource f6(DataDir("TestData/ed25519v1.dat").c_str(), true, new HexDecoder);

		s1.AccessKey().Load(f4);
		s2.AccessKey().Load(f5);
		s3.AccessKey().Load(f6);

		// No throw is success
		fail = false;
	}
	catch (const BERDecodeErr&) {
		fail = true;
	}

	pass = pass && !fail;

	std::cout << (fail ? "FAILED" : "passed") << "  ";
	std::cout << "RFC 5208 and 5958 key loads" << std::endl;

	return pass;
}

NAMESPACE_END  // Test
NAMESPACE_END  // CryptoPP
