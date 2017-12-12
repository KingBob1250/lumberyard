/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

#pragma once

#include <AzCore/Memory/Memory.h>
#include <AzCore/RTTI/RTTI.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/functional.h>

namespace AZ
{
    class SerializeContext;
    void VertexContainerReflect(SerializeContext& context);

    /**
     * A wrapper around a AZStd::vector of either Vector2 or Vector3s.
     * Provides an interface to access and modify the container.
     */
    template <typename Vertex>
    class VertexContainer
    {
    public:
        AZ_TYPE_INFO(VertexContainer, "{39A06CC2-D2C4-4803-A2D1-0E674A61EF4E}", Vertex)
        AZ_CLASS_ALLOCATOR_DECL

        using IndexFunction = AZStd::function<void(size_t)>;
        using VoidFunction = AZStd::function<void()>;

        VertexContainer() = default;
        VertexContainer(const IndexFunction& addCallback, const IndexFunction& removeCallback,
                        const VoidFunction& updateCallback, const VoidFunction& setCallback,
                        const VoidFunction& clearCallback);
        ~VertexContainer() = default;

        /**
         * Add vertex at end of container.
         * @param vertex new vertex position (local space).
         */
        void AddVertex(const Vertex& vertex);
        /**
         * Update position of vertex in container.
         * @param index index of current vertex to update.
         * @param vertex new vertex position (local space).
         * @return was the index in range.
         */
        bool UpdateVertex(size_t index, const Vertex& vertex);
        /**
         * Insert vertex before index in container.
         * @param index index of vertex to insert before.
         * @param vertex new vertex position (local space).
         * @return was the index in range.
         */
        bool InsertVertex(size_t index, const Vertex& vertex);
        /**
         * Remove vertex at index in container.
         * @param index index of vertex to remove.
         * @return was the index in range.
         */
        bool RemoveVertex(size_t index);
        /**
         * Set all vertices.
         * @param vertices new vertices to set.
         */
        template<typename Vertices>
        void SetVertices(Vertices&& vertices);
        /**
         * Remove all vertices.
         */
        void Clear();

        /**
        * Get vertex at index.
        * @return was the index in range.
        */
        bool GetVertex(size_t index, Vertex& vertex) const;
        /**
        * Get last vertex.
        * @param index index of vertex to get.
        * @return if vector is empty return false.
        */
        bool GetLastVertex(Vertex& vertex) const;
        /**
         * Number of vertices in the container.
         */
        size_t Size() const;
        /**
         * Immutable reference to container of vertices.
         */
        const AZStd::vector<Vertex>& GetVertices() const;

        /**
        * Provide callbacks for this container 
        *
        * Useful if you could not provide callbacks at construction or
        * you need to re-supply callbacks after de-serialization
        */
        void SetCallbacks(const IndexFunction& addCallback, const IndexFunction& removeCallback,
                          const VoidFunction& updateCallback, const VoidFunction& setCallback,
                          const VoidFunction& clearCallback);

        static void Reflect(SerializeContext& context);

    private:
        AZStd::vector<Vertex> m_vertices; ///< Vertices (positions).

        AZStd::function<void(size_t)> m_addCallback = nullptr; ///< Callback for when a vertex is added.
        AZStd::function<void(size_t)> m_removeCallback = nullptr; ///< Callback for when a vertex is removed.
        AZStd::function<void()> m_updateCallback = nullptr; ///< Callback for when a vertex is updated/modified.
        AZStd::function<void()> m_setCallback = nullptr; ///< Callback for when a all vertices are set.
        AZStd::function<void()> m_clearCallback = nullptr; ///< Callback for when a all vertices are cleared.

        /**
         * Internal function called when a new vertex is added from the property grid (AZ::Edit::Attributes::AddNotify) - default to Vector3(0,0,0) if empty, else previous last vertex position.
         */
        void AddNotify();
        /**
         * Internal function called when a vertex is removed from the property grid (AZ::Edit::Attributes::RemoveNotify).
         */
        void RemoveNotify(size_t index) const;
        /**
         * Internal function called when a vertex is modified in the property grid (AZ::Edit::Attributes::ChangeNotify).
         */
        void UpdateNotify() const;
    };
}

#include <AzCore/Math/Internal/VertexContainer.inl>