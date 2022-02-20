// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#include <cstdint>

class CCompartment
{
public:
    CCompartment(_In_ IUnknown *punk, TfClientId tfClientId, _In_ REFGUID guidCompartment);
    ~CCompartment();

    HRESULT _GetCompartment(_Outptr_ ITfCompartment **ppCompartment);
    HRESULT _GetCompartmentBOOL(bool& flag);
    HRESULT _SetCompartmentBOOL(bool flag);
    HRESULT _GetCompartmentU32(uint32_t &dw);
    HRESULT _SetCompartmentU32(uint32_t dw);
    HRESULT _ClearCompartment();

    void _GetGUID(GUID* pguid) const;

private:
    void* compartment;
};

typedef HRESULT (*CESCALLBACK)(const void* pv, const GUID* guidCompartment);

class CCompartmentEventSink : public ITfCompartmentEventSink
{
public:
    CCompartmentEventSink(_In_ CESCALLBACK pfnCallback, _In_ void *pv);
    ~CCompartmentEventSink();

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID riid, _Outptr_ void **ppvObj);
    STDMETHODIMP_(ULONG) AddRef(void);
    STDMETHODIMP_(ULONG) Release(void);

    // ITfCompartmentEventSink
    STDMETHODIMP OnChange(_In_ REFGUID guid);

    // function
    HRESULT _Advise(_In_ IUnknown *punk, _In_ REFGUID guidCompartment);
    HRESULT _Unadvise();

private:
    ITfCompartment *_pCompartment;
    DWORD _dwCookie;
    CESCALLBACK _pfnCallback;
    void *_pv;

    LONG _refCount;
};
