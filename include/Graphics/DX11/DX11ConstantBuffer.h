#pragma once
#include <d3d11.h>
#include <wrl.h>

#define DXCALL(hr) if (FAILED(hr)) { std::cerr << "Error at " << __FILE__ << ":" << __LINE__ << " - " << std::hex << hr << std::endl; }

class DX11ConstantBufferBase
{
public:
    virtual ~DX11ConstantBufferBase() = default;
    virtual void Update(ID3D11DeviceContext * context, const void* data) = 0;
    virtual Microsoft::WRL::ComPtr<ID3D11Buffer> GetBuffer() const = 0;
};

template<typename T>
class DX11ConstantBuffer : public DX11ConstantBufferBase
{
public:
    DX11ConstantBuffer(ID3D11Device* device)
    {
        CreateBuffer(device);
    }

    void Update(ID3D11DeviceContext* context, const void* data) override
    {
        UpdateBuffer(context, *reinterpret_cast<const T*>(data));
    }

    Microsoft::WRL::ComPtr<ID3D11Buffer> GetBuffer() const override
    {
        return buffer;
    }

    void UpdateBuffer(ID3D11DeviceContext* context, const T& data)
    {
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        HRESULT hr = context->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        DXCALL(hr);

        memcpy(mappedResource.pData, &data, sizeof(T));

        context->Unmap(buffer.Get(), 0);
    }

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;

    void CreateBuffer(ID3D11Device* device)
    {
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.ByteWidth = (sizeof(T) + 15) & ~15;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        HRESULT hr = device->CreateBuffer(&bufferDesc, nullptr, buffer.GetAddressOf());
        DXCALL(hr);
    }

    
};